/*
 * A063356  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          83 ).
 * https://oeis.org/A063356
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 with initial values
 *            a(2) = -1 a(3) = 247 a(4) = 533 a(5) = 820 a(6) = 1107 a(7) =
 *            1394 a(8) = 1681 a(9) = 1968 a(10) = 2255 a(11) = 2542 a(12) =
 *            2829 a(13) = 3114
 * Range:     i8 n <= 2, i16 n <= 40, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 2) {
        /* Undefined for n < 2; return 0 to avoid UB */
        return (seq_t)0;
    }

    /* Ensure the cache is large enough */
    idx_t required = n + 1;   /* we use index directly */
    if (required > cache_size) {
        /* Grow the cache */
        size_t new_len = (size_t)required;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (cache == NULL) {   /* allocation failed */
            abort();
        }
        /* Initialise new entries to 0 */
        for (idx_t i = cache_size; i < required; ++i) {
            cache[i] = (seq_t)0;
        }
        cache_size = required;
    }

    /* If already computed, return */
    if (cache[n] != (seq_t)0 || n < 2) {
        return cache[n];
    }

    /* Set the known initial values up to n = 13 */
    if (n == 2) return cache[n] = (seq_t)-1;
    if (n == 3) return cache[n] = (seq_t)247;
    if (n == 4) return cache[n] = (seq_t)533;
    if (n == 5) return cache[n] = (seq_t)820;
    if (n == 6) return cache[n] = (seq_t)1107;
    if (n == 7) return cache[n] = (seq_t)1394;
    if (n == 8) return cache[n] = (seq_t)1681;
    if (n == 9) return cache[n] = (seq_t)1968;
    if (n ==10) return cache[n] = (seq_t)2255;
    if (n ==11) return cache[n] = (seq_t)2542;
    if (n ==12) return cache[n] = (seq_t)2829;
    if (n ==13) return cache[n] = (seq_t)3114;

    /* Compute values upward from current cache size if needed */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != (seq_t)0 || i < 2) continue;  /* skip already set */
        if (i <= 13) {
            /* These should have been handled above */
            continue;
        }
        cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
    }

    return cache[n];
}
