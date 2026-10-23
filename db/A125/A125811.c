/*
 * A125811  Number of coefficients in the n-th q-Bell number as a polynomial
 *          in q.
 * https://oeis.org/A125811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A023536(n-2) + 1 Equivalent recurrence for n ≥ 3:
 *            a(0)=a(1)=a(2)=1 a(n)=a(n-1)+n- floor(½+√(2*(n-2)+4))
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

static seq_t *cache = NULL;      /* memoised values, indexed by n */
static idx_t  cache_size = 0;    /* number of elements currently stored */

/* Compute floor(½+√x) using only integer arithmetic.
   The 64‑bit intermediate type ensures the test does not overflow
   when seq_t is narrower than 64 bits. */
static seq_t round_sqrt_half_plus_half(seq_t x)
{
    seq_t s = floor_sqrt(x);                /* floor(√x) */
    idx_t s_idx = (idx_t)s;
    idx_t x_idx = (idx_t)x;
    idx_t cmp   = s_idx * s_idx + s_idx;    /* s² + s */
    return (x_idx > cmp) ? (s + 1) : s;
}

seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n. */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp && n + 1 > 0) abort();     /* allocation failure */
        cache = tmp;
    }

    /* Compute any values not yet memoised. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 2) {
            cache[i] = 1;            /* a(0)=a(1)=a(2)=1 */
        } else {
            /* r = floor(½+√(2*(i-2)+4)) */
            seq_t t = (seq_t)2 * (seq_t)(i - 2) + (seq_t)4;
            seq_t r = round_sqrt_half_plus_half(t);
            cache[i] = cache[i - 1] + (seq_t)i - r;
        }
    }

    cache_size = n + 1;              /* all indices up to n are ready */
    return cache[n];
}
