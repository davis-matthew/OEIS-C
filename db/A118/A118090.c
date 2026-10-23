/*
 * A118090  Start with 1 and repeatedly reverse the digits and add 44 to get
 *          the next term.
 * https://oeis.org/A118090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1, a(n) = reverse_digits(a(n−1)) + 44 for n > 1
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* reverse_digits is implemented by repeated division and multiplication by 10. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 0 is undefined, return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored (>=0) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure capacity for at least n terms */
    if ((size_t)n > cache_cap) {
        /* Allocate or grow the array */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – return 0 or handle error as preferred */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_cap = (size_t)n;
    }

    /* Initialise first term if the cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;      /* a(1) = 1 */
        cache_len = 1;
    }

    /* Compute missing terms up to n */
    for (size_t idx = cache_len; idx < (size_t)n; ++idx) {
        /* Reverse the previous term */
        seq_t rev = (seq_t)0;
        seq_t x = cache[idx - 1];
        while (x > (seq_t)0) {
            seq_t digit = x % (seq_t)10;
            rev = rev * (seq_t)10 + digit;
            x = x / (seq_t)10;
        }
        cache[idx] = rev + (seq_t)44;
    }
    cache_len = (size_t)n;

    return cache[(size_t)n - 1];
}
