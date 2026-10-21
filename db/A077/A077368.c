/*
 * A077368  Triangular numbers whose external digits as well as internal
 *          digits form triangular numbers.
 * https://oeis.org/A077368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th triangular number T_k such that * 1. the
 *            two‑digit number formed by its first and last decimal * digits
 *            is triangular, * 2. the number formed by its internal digits
 *            (the digits * except the first and last) is triangular. * (For
 *            one‑digit numbers this number is 0, which is triangul...
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *seq_cache = NULL;     /* cache of found terms            */
static size_t   cache_cap  = 0;      /* current capacity of seq_cache   */
static size_t   cache_len  = 0;      /* number of valid terms in cache  */
static idx_t   next_k = 0;           /* next value of k to process      */

/* Test whether a non‑negative seq_t x is triangular.  */
static int is_triangular(seq_t x)
{
    if (x == 0) return 1;
    seq_t d   = 1 + (seq_t)8 * x;
    seq_t s   = floor_sqrt(d);
    if (s * s != d) return 0;
    seq_t m   = (s - 1) / 2;
    return (m * (m + 1) / 2) == x;
}

/* Expand the cache until it contains at least n terms. */
static void grow_cache_for(idx_t n)
{
    if (cache_len >= (size_t)n) return;  /* already have enough */

    /* Allocate a new array if needed.  */
    if (cache_cap == 0) {
        cache_cap = 64;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
    }

    /* Generate triangular numbers T_k for k >= next_k until we have n terms. */
    while (cache_len < (size_t)n) {
        /* Compute the k‑th triangular number T_k = k*(k+1)/2. */
        idx_t k = next_k;
        seq_t T  = ((seq_t)k) * ((seq_t)k + 1) / 2;

        /* Compute the last decimal digit. */
        seq_t last = T % 10;

        /* Determine the first decimal digit and the power 10^(d-1). */
        seq_t pow10 = 1;
        while (pow10 <= T / 10) pow10 *= 10;   /* pow10 = 10^(d-1) */
        seq_t first = T / pow10;

        /* Form the external two‑digit number. */
        seq_t ext;
        if (pow10 == 1)      /* single digit number */
            ext = T;
        else
            ext = first * 10 + last;

        /* Internal digits form the number obtained by removing first and last digits. */
        seq_t internal = 0;
        if (pow10 > 10) {    /* d >= 3 */
            internal = (T / 10) % (pow10 / 10);
        }

        /* Check both conditions. */
        if (is_triangular(ext) && is_triangular(internal)) {
            /* Store it in the cache. */
            seq_cache[cache_len++] = T;
        }

        ++next_k;
    }
}

/* External linkage required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined below offset, but guard */
    grow_cache_for(n);
    return seq_cache[n - 1];
}
