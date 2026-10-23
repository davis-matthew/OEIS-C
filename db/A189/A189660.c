/*
 * A189660  Partial sums of A064990.
 * https://oeis.org/A189660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   s(n) = s(floor((n+2)/3)) + s(floor((n+1)/3)) + floor(n/3) -
 *            s(floor(n/3)), with s(0) = 0, s(1) = 0. This function returns
 *            the value s(n) for n >= 1 (OEIS A189660)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* realloc */

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size (indices 0..cache_size-1) */
    static idx_t  current_max = 0;  /* greatest index computed so far */
    static int    initialized = 0;  /* 0 = not yet initialized  */

    /* Handle n == 0 (not required by the offset but needed for the recurrence) */
    if (n == 0)
        return (seq_t)0;

    /* Lazy initialization of the cache */
    if (!initialized) {
        /* need room for indices 0 and 1 at least */
        cache_size = 2;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));  /* +1 to be safe */
        if (!cache) return (seq_t)0;  /* allocation failed, return 0 as safe fallback */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        current_max = 1;
        initialized = 1;
    }

    /* Grow the cache if we need more space for indices up to n */
    if ((size_t)n > cache_size) {
        size_t new_size = cache_size * 2;
        if ((size_t)n > new_size)
            new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute new values up to n (if not already computed) */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        /* The recurrence is valid for i >= 2; for i==1 we already have base value 0 */
        seq_t val;
        if (i == 1) {
            val = (seq_t)0;   /* base case, already set */
        } else {
            idx_t a_idx = (i + 2) / 3;
            idx_t b_idx = (i + 1) / 3;
            idx_t c_idx = i / 3;

            val = cache[a_idx] + cache[b_idx] + (seq_t)c_idx - cache[c_idx];
        }
        cache[i] = val;
    }
    current_max = n;

    return cache[n];
}
