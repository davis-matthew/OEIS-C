/*
 * A153373  Number of zig-zag paths from top to bottom of a rectangle of width
 *          11 with 2n-1 rows whose color is not that of the top right corner.
 * https://oeis.org/A153373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(1) = 5, a(2) = 18, a(3) = 66 * a(n) = 6*a(n-1) - 9*a(n-2) +
 *            2*a(n-3) for n >= 4 * (order‑3 linear recurrence with constant
 *            coefficients)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of computed values; cache[0] holds a(1). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms currently stored */

/* Ensure that cache contains at least n terms (n >= 1). */
static void ensure_cache(idx_t n)
{
    if (n == 0 || n <= cache_len)
        return;   /* nothing to do */

    idx_t new_len = n;
    idx_t old_len = cache_len;

#if defined(__cplusplus)
/* Allocate new array with the C++ new operator (works also when
   seq_t is a built‑in type).  Copy existing entries if any. */
    seq_t *new_cache = new seq_t[new_len];
    if (old_len > 0) {
        for (idx_t i = 0; i < old_len; ++i)
            new_cache[i] = cache[i];
    }
    delete[] cache;   /* delete old array if it exists */
    cache = new_cache;
#else
/* Allocate (or reallocate) with realloc for the C build. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {
        abort();   /* allocation failure */
    }
    cache = new_cache;
#endif

    cache_len = old_len;   /* will be updated when filling */

    /* Fill the first three terms if they are not present. */
    if (cache_len < 1 && new_len >= 1) {
        cache[0] = (seq_t)5;
        cache_len = 1;
    }
    if (cache_len < 2 && new_len >= 2) {
        cache[1] = (seq_t)18;
        cache_len = 2;
    }
    if (cache_len < 3 && new_len >= 3) {
        cache[2] = (seq_t)66;
        cache_len = 3;
    }

    /* Compute remaining terms using the recurrence. */
    while (cache_len < new_len) {
        seq_t a1 = cache[cache_len - 1];      /* a(n-1) */
        seq_t a2 = cache[cache_len - 2];      /* a(n-2) */
        seq_t a3 = cache[cache_len - 3];      /* a(n-3) */
        cache[cache_len] = 6 * a1 - 9 * a2 + 2 * a3;
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1, but return 0 as safe value */

    ensure_cache(n);
    return cache[n - 1];
}
