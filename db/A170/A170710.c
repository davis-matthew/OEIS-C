/*
 * A170710  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170710:
   a(0) = 1
   a(n) = 27*∑_{k=1}^{49} a(n-k) + 2            for 1 ≤ n ≤ 49
   a(50) = 27*∑_{k=1}^{49} a(50-k) – 378*a(0) + 1
   a(n) = 27*∑_{k=1}^{49} a(n-k) – 378*a(n-50)   for n ≥ 51
   (a(m) = 0 for m < 0)
*/

#include <stdlib.h>

static seq_t *cache = NULL;        /* cache of a(n) values   */
static idx_t  cache_len = 0;       /* number of terms stored */
static seq_t  sum_last49 = 0;      /* S(i) for the next i   */

seq_t a(idx_t n)
{
    /* Grow the cache if needed. */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        /* cache_len is still the old length; it will be updated after we
           finish computing the missing terms. */
    }

    /* Compute terms that are not yet known. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Constant term C(i). */
        seq_t c;
        if (i == 0 || i == 50)
            c = (seq_t)1;
        else if (i <= 49)
            c = (seq_t)2;
        else
            c = (seq_t)0;

        /* a(i) = 27*S(i) + C(i) – 378*a(i-50) (if i ≥ 50). */
        seq_t term = sum_last49 * (seq_t)27 + c;
        if (i >= 50)
            term -= cache[i - 50] * (seq_t)378;

        cache[i] = term;

        /* Update S(i+1) for the next iteration:
           S(i+1) = S(i) + a(i) – a(i+1-50) (if i+1 ≥ 50). */
        seq_t sub = ((i + 1) >= 50) ? cache[i + 1 - 50] : (seq_t)0;
        sum_last49 += cache[i];
        sum_last49 -= sub;
    }

    /* Now the cache is up‑to‑date. */
    cache_len = n + 1;
    return cache[n];
}
