/*
 * A218517  Number of n X 3 arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random 0..3 n X 3 array.
 * https://oeis.org/A218517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 19*a(n-2) - 96*a(n-3) - 112*a(n-4) - a(n-5) +
 *            140*a(n-6) + 64*a(n-7) Initial values: a(1) = 16, a(2) = 286,
 *            a(3) = 4634, a(4) = 76214, a(5) = 1254462, a(6) = 20655650, a(7)
 *            = 340133516
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
#  define CONST_SEQ_T constexpr
#else
#  define CONST_SEQ_T const
#endif

/* Provide proper initialization of the initial values for both C and C++ */
#if defined(__cplusplus)
static const seq_t init_vals[7] = {
    seq_t(16),      seq_t(286),      seq_t(4634),
    seq_t(76214),   seq_t(1254462),  seq_t(20655650),
    seq_t(340133516)
};
#else
static const seq_t init_vals[7] = {
    16, 286, 4634, 76214, 1254462, 20655650, 340133516
};
#endif

/* Cache for previously computed terms.
   cache[0] is unused to keep indices 1..n directly. */
static seq_t *cache = NULL;
static idx_t cache_max = 0;

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;                    /* not used (offset = 1) */

    /* Handle the first 7 terms directly */
    if (n <= 7)
        return init_vals[n - 1];

    /* Grow cache if necessary */
    if (n > cache_max) {
        size_t new_size = (size_t)n + 1;     /* room for indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();                        /* allocation failure */
        cache = tmp;

        /* Initialise entries 1..7 if they haven't been set yet */
        for (idx_t i = cache_max + 1; i <= n && i <= 7; ++i)
            cache[i] = init_vals[i - 1];

        /* Compute new terms from max(cache_max+1,8) up to n */
        idx_t start = cache_max + 1;
        if (start < 8)
            start = 8;

        for (idx_t i = start; i <= n; ++i) {
            /* Positive contributions */
            seq_t pos = cache[i - 1] * 18;
            pos += cache[i - 6] * 140;
            pos += cache[i - 7] * 64;

            /* Negative contributions */
            seq_t neg = cache[i - 2] * 19;
            neg += cache[i - 3] * 96;
            neg += cache[i - 4] * 112;
            neg += cache[i - 5];                /* *1 */

            cache[i] = pos - neg;               /* pos >= neg for all n>=8 */
        }

        cache_max = n;
    }

    return cache[n];
}
