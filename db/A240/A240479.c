/*
 * A240479  Number of n X 3 0..1 arrays with no element equal to more than two
 *          horizontal or vertical neighbors, with new values 0..1 introduced
 *          in row major order.
 * https://oeis.org/A240479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the recurrence a(n) = 4*a(n-1) + 3*a(n-2) +
 *            2*a(n-3) + 6*a(n-4) + 2*a(n-5) - a(n-6) Initial values (for n =
 *            1 … 6): a(1) = 4, a(2) = 25, a(3) = 122, a(4) = 578, a(5) =
 *            2753, a(6) = 13147
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free                 */
#include <string.h>   /* memcpy (not used in C++)     */

/* Static cache for computed values a(1) … a(k) */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of cached terms */

/* Base values for n = 1 … 6 */
static const seq_t base[6] = {
    (seq_t)4,   /* a(1) */
    (seq_t)25,  /* a(2) */
    (seq_t)122, /* a(3) */
    (seq_t)578, /* a(4) */
    (seq_t)2753,/* a(5) */
    (seq_t)13147/* a(6) */
};

seq_t a(idx_t n)
{
    /* Guard against an illegal call */
    if (n == 0)
        return (seq_t)0;

    /* Grow the cache if necessary */
    if (cache_size < (size_t)n) {
        size_t new_size = (size_t)n;

        /* Allocate new cache array */
#if defined(__cplusplus)
        seq_t *new_cache = new seq_t[new_size];
#else
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
#endif
        if (!new_cache)
            return (seq_t)0;  /* allocation failure */

        /* Copy old values into the new array */
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
#if defined(__cplusplus)
            delete[] cache;
#else
            free(cache);
#endif
        }

        /* Compute missing terms */
        for (size_t i = cache_size + 1; i <= new_size; ++i) {
            size_t idx = i - 1;                 /* zero‑based index          */
            if (i <= 6) {
                new_cache[idx] = base[idx];
            } else {
                /* Apply the recurrence */
                seq_t val = 4 * new_cache[idx - 1];
                val += 3 * new_cache[idx - 2];
                val += 2 * new_cache[idx - 3];
                val += 6 * new_cache[idx - 4];
                val += 2 * new_cache[idx - 5];
                val -= new_cache[idx - 6];
                new_cache[idx] = val;
            }
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n - 1];
}
