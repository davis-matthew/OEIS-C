/*
 * A063318  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          45 ).
 * https://oeis.org/A063318
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n>13 Initial values: a(2) =
 *            -1, a(3) = 39, a(4) = 92, a(5) = 143, a(6) = 194, a(7) = 249,
 *            a(8) = 300, a(9) = 351, a(10) = 406, a(11) = 457, a(12) = 508,
 *            a(13) = 561 a(0) and a(1) are defined as 0 (unused in the OEIS
 *            offset)
 * Range:     i8 n <= 4, i16 n <= 44, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoisation array */
static size_t cache_len = 0;       /* allocated length of cache */

/* Initial values for indices 0…13 (a(0)=a(1)=0 for convenience) */
static const seq_t init_vals[14] = {
    0,          /* a(0) */
    0,          /* a(1) */
    -1,         /* a(2) */
    39,         /* a(3) */
    92,         /* a(4) */
    143,        /* a(5) */
    194,        /* a(6) */
    249,        /* a(7) */
    300,        /* a(8) */
    351,        /* a(9) */
    406,        /* a(10) */
    457,        /* a(11) */
    508,        /* a(12) */
    561         /* a(13) */
};

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2; behaviour for smaller n is irrelevant. */
    if (n < 2)
        return 0;

    size_t idx = (size_t)n;

    /* Extend the cache to hold the requested index. */
    if (idx >= cache_len) {
        size_t new_len = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)          /* out‑of‑memory; caller may treat as 0 */
            return 0;
        cache = tmp;

        /* Initialise new entries. */
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i <= 13) {
                /* Use the known initial values. */
                cache[i] = init_vals[i];
            } else {
                /* Recurrence: a(i) = a(i-4) + a(i-6) - a(i-10). */
                cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
            }
        }
        cache_len = new_len;
    }

    return cache[idx];
}
