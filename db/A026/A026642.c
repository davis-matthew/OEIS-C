/*
 * A026642  a(n) = A026637(2*n-1, n-2).
 * https://oeis.org/A026642
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((7*n^2 - 11*n + 6)*a(n-1) +
 *            2*(n-1)*(2*n-1)*a(n-2))/(2*(n-2)*(n+1)) Initial values: a(2)=1,
 *            a(3)=7
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t max_cached = 1;          /* highest n computed so far */

/* Ensure that cache contains values up to n */
static void ensure_up_to(idx_t n)
{
    if (n <= max_cached)
        return;

    /* Allocate or grow cache */
    if (cache_size == 0) {
        cache_size = n + 1 < 5 ? 5 : n + 1;   /* at least 5 to hold indices 0..4 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return;   /* out of memory: silently fail, result will be garbage */
        cache[0] = cache[1] = 0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)7;
        max_cached = 3;
    }
    if (cache_size <= n) {
        size_t new_size = cache_size * 2;
        if (new_size <= n)
            new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cache_size = new_size;
        } else {
            return;   /* out of memory */
        }
    }

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i < 4) {          /* base values already set */
            continue;
        }
        seq_t a_im1 = cache[i - 1];
        seq_t a_im2 = cache[i - 2];

        if (sizeof(seq_t) <= 8) {   /* use 128‑bit arithmetic for 64‑bit seq_t */
            __int128 idx = ( __int128 ) i;
            __int128 coeff1 = 7 * idx * idx - 11 * idx + 6;            /* 7i²-11i+6 */
            __int128 coeff2 = 2 * (idx - 1) * (2 * idx - 1);           /* 2(i‑1)(2i‑1) */
            __int128 num1   = coeff1 * ( __int128 ) a_im1;
            __int128 num2   = coeff2 * ( __int128 ) a_im2;
            __int128 den    = 2 * (idx - 2) * (idx + 1);
            __int128 cur    = (num1 + num2) / den;
            cache[i] = (seq_t) cur;
        } else {                /* seq_t is large enough – use its arithmetic */
            seq_t coeff1 = (seq_t)7 * (seq_t)i * (seq_t)i
                          - (seq_t)11 * (seq_t)i
                          + (seq_t)6;
            seq_t coeff2 = (seq_t)2 * ((seq_t)(i - 1))
                          * ((seq_t)(2 * i - 1));
            seq_t term1   = coeff1 * a_im1;
            seq_t term2   = coeff2 * a_im2;
            seq_t numer   = term1 + term2;
            seq_t denom   = (seq_t)2 * ((seq_t)(i - 2)) * ((seq_t)(i + 1));
            cache[i] = numer / denom;
        }
    }
    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* undefined for indices below the offset */

    ensure_up_to(n);

    return cache[n];
}
