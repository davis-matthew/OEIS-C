/*
 * A063341  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          68 ).
 * https://oeis.org/A063341
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n>13 Initial values: a(2) =
 *            -1 a(3) = 68 a(4) = 152 a(5) = 236 a(6) = 318 a(7) = 402 a(8) =
 *            484 a(9) = 572 a(10) = 650 a(11) = 738 a(12) = 816 a(13) = 906
 * Range:     i8 n <= 3, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t init_vals[] = {
    (seq_t)-1,
    (seq_t)68,
    (seq_t)152,
    (seq_t)236,
    (seq_t)318,
    (seq_t)402,
    (seq_t)484,
    (seq_t)572,
    (seq_t)650,
    (seq_t)738,
    (seq_t)816,
    (seq_t)906
};

static seq_t *cache = NULL;   /* cache[i] holds a(i+2) */
static size_t cache_len = 0;
static size_t cache_cap = 0;
static bool cache_inited = false;

seq_t a(idx_t n) {
    if (n < (idx_t)2)
        return (seq_t)0;  /* undefined below offset, arbitrary 0 */

    /* initialise cache with the first 12 terms (a(2) .. a(13)) */
    if (!cache_inited) {
        cache_inited = true;
        cache_cap = 64;  /* arbitrary grow factor */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < sizeof(init_vals)/sizeof(init_vals[0]); ++i)
            cache[i] = init_vals[i];
        cache_len = sizeof(init_vals)/sizeof(init_vals[0]);  /* 12 */
    }

    /* if n is within the initial segment, return directly */
    if (n <= (idx_t)13)
        return cache[(size_t)(n - 2)];

    /* ensure cache is large enough for index n-2 */
    size_t needed_len = (size_t)(n - 1);   /* number of elements needed */
    if (needed_len > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed_len) new_cap = needed_len;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (size_t idx = cache_len; idx < needed_len; ++idx) {
        cache[idx] = cache[idx - 4] + cache[idx - 6] - cache[idx - 10];
    }
    cache_len = needed_len;

    return cache[(size_t)(n - 2)];
}
