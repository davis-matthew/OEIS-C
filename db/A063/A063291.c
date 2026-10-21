/*
 * A063291  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          18 ).
 * https://oeis.org/A063291
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n>13 * Initial values (for
 *            2 ≤ n ≤ 13): * a(2) = -1, a(3) = 2, a(4) = 6, a(5) = 7, a(6) =
 *            8, * a(7) = 13, a(8) = 14, a(9) = 16, a(10) = 20, * a(11) = 22,
 *            a(12) = 22, a(13) = 27
 * Range:     i8 n <= 54, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoization array */
static idx_t cache_capacity = 0;     /* allocated size of cache (highest usable index) */
static idx_t computed_upto = 1;      /* largest n that has been computed (initialized to 1 → none) */

/* Small table for the first few values */
static const seq_t INIT[14] = {
    0, 0,          /* indices 0 and 1 unused    */
    -1, 2, 6, 7, 8, 13, 14, 16, 20, 22, 22, 27
};

/* Ensure the cache has room for indices up to n */
static void ensure_capacity(idx_t n)
{
    if (cache_capacity <= n) {
        idx_t new_cap = (cache_capacity == 0) ? 32 : cache_capacity;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache_capacity = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;        /* behaviour below offset is irrelevant */

    ensure_capacity(n);

    if (n <= computed_upto)
        return cache[n];

    /* Compute values in increasing order up to n */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        if (i <= 13) {                    /* use pre‑computed constants */
            cache[i] = INIT[i];
        } else {                          /* recurrence */
            cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
        }
    }
    computed_upto = n;
    return cache[n];
}
