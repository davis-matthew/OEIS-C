/*
 * A119956  Numbers n such that n^3+1=p*q*r where p,q,r are distinct primes.
 * https://oeis.org/A119956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that n^3+1 is the product of three distinct primes */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Static cache to memoise discovered terms */
static seq_t *cache = NULL;
static size_t len   = 0;
static size_t cap   = 0;
static uint64_t last_checked = 1;   /* next integer to test */

static void ensure_capacity(void)
{
    if (len == cap) {
        size_t newcap = cap ? cap * 2 : 32;
        seq_t *newarr = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!newarr) abort();
        if (cache) {
            memcpy(newarr, cache, len * sizeof(seq_t));
            free(cache);
        }
        cache = newarr;
        cap   = newcap;
    }
}

/* Return the nth term (1‑based) of A119956 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range, safeguard */

    /* extend cache until it contains at least n terms */
    while (len < (size_t)n) {
        uint64_t m = last_checked++;
        uint64_t v = m * m * m + 1;          /* compute m^3+1 */

        uint64_t pr[64], ex[64];
        size_t   fp;
        factor_u64(v, pr, ex, &fp);          /* factor v */

        if (fp == 3) {                       /* exactly three distinct primes? */
            bool ok = true;
            for (size_t i = 0; i < fp; ++i) {
                if (ex[i] != 1) { ok = false; break; }
            }
            if (ok) {
                ensure_capacity();
                cache[len++] = (seq_t)m;
            }
        }
    }

    return cache[n - 1];
}
