/*
 * A188382  Primes of the form 8*n^2 + 2*n + 1.
 * https://oeis.org/A188382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 8*m^2 + 2*m + 1, where m ≥ 1
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 921 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Compute the nth term of A188382 for n ≥ 1. */
seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static idx_t next_m = 1;   /* m to test next */

    if (n == 0) {
        /* undefined below the offset; return 0 for safety */
        return (seq_t)0;
    }

    /* Extend cache until it contains at least n primes */
    while (primes_len < n) {
        /* compute candidate p = 8*m^2 + 2*m + 1 using 64‑bit arithmetic */
        uint64_t m64 = (uint64_t)next_m;
        uint64_t p64 = m64 * m64 * 8ULL + m64 * 2ULL + 1ULL;

        /* cast to seq_t (safe for values that fit; otherwise wrap) */
        seq_t p = (seq_t)p64;

        if (is_prime_big(p)) {
            /* grow cache if needed */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                if (!tmp) {
                    /* out of memory – return 0 as a graceful failure */
                    return (seq_t)0;
                }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = p;
        }
        ++next_m;
    }

    /* return the required term (1‑based indexing) */
    return primes[n - 1];
}
