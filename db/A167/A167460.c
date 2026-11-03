/*
 * A167460  Primes p such that (p reversed)+6 is also a prime.
 * https://oeis.org/A167460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = primes p for which reverse(p) + 6 is also prime
 * Range:     u8 n <= 13, u16 n <= 286, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 959 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity of cache */
    static size_t len   = 0;   /* number of primes stored */
    static idx_t  next  = 2;   /* next integer to test */

    while (len < (size_t)n) {
        idx_t p = next++;

        /* skip even numbers greater than 2 */
        if (p > 2 && (p & 1) == 0)
            continue;

        if (!is_prime_big((seq_t)p))
            continue;

        /* decimal reverse of p */
        idx_t revp = 0;
        idx_t temp = p;
        while (temp > 0) {
            revp = revp * 10 + (temp % 10);
            temp /= 10;
        }

        seq_t candidate = (seq_t)revp + (seq_t)6;
        if (!is_prime_big(candidate))
            continue;

        /* store the found prime */
        if (len == cap) {
            size_t new_cap = cap ? cap << 1 : 8;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();     /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }
        cache[len++] = (seq_t)p;
    }

    return cache[n - 1];
}
