/*
 * A142183  Primes congruent to 1 mod 40.
 * https://oeis.org/A142183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 388, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 978 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes ≡ 1 (mod 40), offset 1.  Computed by incremental search
   of candidates. */
#include <stddef.h>
#include <stdbool.h>
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
    /* Static cache of primes congruent to 1 modulo 40. */
    static seq_t *primes = NULL;
    static size_t  cap   = 0;
    static idx_t   len   = 0;
    static seq_t last = 0;   /* last prime stored */

    /* Allocate initial storage if needed. */
    if (primes == NULL) {
        cap = 256;
        primes = mem_new(seq_t, cap);
        if (primes == NULL) {
            /* Allocation failed – cannot continue. */
            exit(EXIT_FAILURE);
        }
    }

    /* Generate further primes until we reach the requested index. */
    while (len < n) {
        seq_t candidate;

        if (len == 0) {
            /* First prime in the sequence. */
            candidate = (seq_t)41;
        } else {
            /* Next candidate must be 40 higher than the last found prime. */
            candidate = last;
            candidate += (seq_t)40;
            while (!is_prime_big(candidate)) {
                candidate += (seq_t)40;
            }
        }

        /* Grow cache if necessary. */
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = mem_resize(seq_t, primes, new_cap);
            if (tmp == NULL) {
                /* Re‑allocation failed – clean up and terminate. */
                mem_free(primes);
                exit(EXIT_FAILURE);
            }
            primes = tmp;
            cap = new_cap;
        }

        primes[len++] = candidate;
        last = candidate;
    }

    return primes[n - 1];
}
