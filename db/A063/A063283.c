/*
 * A063283  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          10 ).
 * https://oeis.org/A063283
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n>13, with initials: a(2) =
 *            -1, a(3) = 0, a(4) = 2, a(5) = 3, a(6) = 4, a(7) = 5, a(8) = 6,
 *            a(9) = 5, a(10) = 8, a(11) = 7, a(12) = 10, a(13) = 11. All
 *            other terms follow the recurrence
 * Range:     i8 n <= 68, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t init_vals[14] = {
    [0] = 0,          /* dummy for index 0 */
    [1] = 0,          /* dummy for index 1 */
    [2] = (seq_t)-1,  /* a(2) */
    [3] = 0,          /* a(3) */
    [4] = 2,          /* a(4) */
    [5] = 3,          /* a(5) */
    [6] = 4,          /* a(6) */
    [7] = 5,          /* a(7) */
    [8] = 6,          /* a(8) */
    [9] = 5,          /* a(9) */
    [10] = 8,         /* a(10) */
    [11] = 7,         /* a(11) */
    [12] = 10,        /* a(12) */
    [13] = 11         /* a(13) */
};

static seq_t *cache = NULL;           /* cached values */
static idx_t cache_size = 0;          /* allocated length of cache */
static idx_t max_computed = 1;        /* highest index computed so far */

/* Ensure cache has room for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;
    seq_t *new_cache = (seq_t *)realloc(cache,
                                         sizeof(seq_t) * (size_t)(n + 1));
    if (!new_cache) return;          /* out of memory (unlikely) */
    cache = new_cache;
    cache_size = n + 1;
}

/* Main sequence access function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* behaviour below offset is irrelevant */

    /* Allocate or grow cache if needed */
    ensure_cache(n);

    /* Compute missing values up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i <= 13) {
                cache[i] = init_vals[i];
            } else {
                cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
            }
        }
        max_computed = n;
    }

    return cache[n];
}
