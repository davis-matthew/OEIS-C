/*
 * A101284  Numbers n such that a type-8 Gaussian normal basis exists for
 *          GF(2^n) over GF(2).
 * https://oeis.org/A101284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   distinct_prime_factors, is_prime, mod_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine implements OEIS A101284.
   For an integer n ≥ 1 it tests whether n is a
   “type‑8 Gaussian normal basis” for GF(2ⁿ) over GF(2).
   A basis exists iff  p = 8n + 1  is prime and
   gcd(n, (p‑1)/ord₂(p)) = 1,
   where ord₂(p) is the multiplicative order of 2 modulo p.
   The function builds the sequence lazily and memoizes all
   terms that have been produced so far.  It relies on the
   utility functions is_prime(…) and the standard
   arithmetic operators only.  The algorithm runs in
   roughly O(max(n)) time for n up to about 100 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime.h"
#include "../utils/distinct_prime_factors.h"
#include "../utils/mod_pow2.h"

/* Euclidean gcd for idx_t (unsigned 64‑bit). */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;   /* number of stored terms  (≥ 0) */
    static idx_t cache_cap = 0;   /* allocated capacity          */
    static idx_t next_n = 1;      /* next trial n value          */

    /* Allocate or enlarge cache as needed. */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
    }

    /* Generate terms until we have at least n of them. */
    while (cache_cnt < n) {
        /* Test candidate n = next_n. */
        uint64_t p = 8ULL * next_n + 1ULL;

        if (is_prime(p)) {
            /* Compute ord₂(p).  It divides p‑1 = 8·next_n.
               Start with the full exponent and reduce by prime factors. */
            uint64_t ord = p - 1;

            /* Gather distinct prime factors of (p‑1). */
            idx_t fac[32];
            size_t fac_len = 0;
            distinct_prime_factors(p - 1, fac, 32, &fac_len);

            /* Reduce the order. */
            for (size_t i = 0; i < fac_len; ++i) {
                idx_t q = fac[i];
                while (ord % q == 0 && mod_pow2(ord / q, p) == 1)
                    ord /= q;
            }

            uint64_t k = (p - 1) / ord;
            if (gcd_idx(next_n, k) == 1) {
                /* Store this qualifying n. */
                if (cache_cnt >= cache_cap) {
                    cache_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (tmp == NULL) exit(EXIT_FAILURE);
                    cache = tmp;
                }
                cache[cache_cnt++] = (seq_t)next_n;
            }
        }
        ++next_n;
    }

    return cache[n - 1];   /* 1‑based indexing */
}
