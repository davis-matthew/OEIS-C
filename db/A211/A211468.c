/*
 * A211468  Number of (n+1)X(n+1) -8..8 symmetric matrices with every 2X2
 *          subblock having sum zero and one or three distinct values.
 * https://oeis.org/A211468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 3*a(n-1)+13*a(n-2)-43*a(n-3)-68*a(n-4)+255*a(n-5)+185*a(n-6)
 *              -810*a(n-7)-289*a(n-8)+1487*a(n-9)+292*a(n-10)-1577*a(n-11)-242*a(n-12)
 *              +895*a(n-13)+170*a(n-14)-210*a(n-15)-60*a(n-16)
 *  Initial terms (n=1..16): 121,277,565,1101,2135,4047,7739,14661,28031,53463,
 * 102733,197579,382143,740723,1441747,2813157
 */

#include <stdlib.h>

static const int coeffs[16] = {
    3, 13, -43, -68, 255, 185, -810, -289,
    1487, 292, -1577, -242, 895, 170, -210, -60
};

static const seq_t init[16] = {
    121, 277, 565, 1101, 2135, 4047,
    7739, 14661, 28031, 53463, 102733,
    197579, 382143, 740723, 1441747, 2813157
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of stored terms (>=1) */

    /* initialise cache with first 16 terms */
    if (cache_size == 0) {
        cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = init[i];
    }

    /* return if already computed */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* extend cache up to n */
    size_t new_size = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – aborting */
        exit(EXIT_FAILURE);
    }
    cache = tmp;

    for (size_t i = cache_size + 1; i <= new_size; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        size_t idx = i - 1;  /* zero‑based index of the new term */
        for (int k = 0; k < 16; ++k) {
            size_t prev = idx - (k + 1);          /* i - k - 1 (zero based) */
            int c = coeffs[k];
            if (c >= 0) {
                pos += cache[prev] * (seq_t)c;
            } else {
                neg += cache[prev] * (seq_t)(-c);
            }
        }
        cache[idx] = pos - neg;  /* positive for all valid n */
    }
    cache_size = new_size;
    return cache[n - 1];
}
