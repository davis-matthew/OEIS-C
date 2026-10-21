/*
 * A218657  Number of n X 2 binary arrays with every 0 a king-move neighbor to
 *          some 1.
 * https://oeis.org/A218657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + 3*a(n-3) for n > 3, with initial
 *            values a(1)=3, a(2)=15, a(3)=57
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* External types provided by the build system. */
extern seq_t a(idx_t n);

/* Cached sequence values. */
static seq_t *cache = NULL;          /* pointer to allocated array */
static idx_t   cache_len = 0;        /* number of values currently stored */
static idx_t   cache_cap = 0;        /* capacity of the allocated array */

seq_t a(idx_t n)
{
    /* Sanity check: the sequence is defined for n >= 1 */
    if (n < 1) {
        return 0;
    }

    /* Ensure the cache is large enough to hold up to the requested n. */
    if (n > cache_len) {
        /* Compute new capacity: double until it fits. */
        idx_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
        while (new_cap < n) {
            if (new_cap > (sizeof(size_t) == 8 ? (size_t)-1 >> 1 : (size_t)-1 >> 1)) {
                /* This should never happen for reasonable n. */
                break;
            }
            new_cap <<= 1; /* double the capacity */
        }
        /* Reallocate the cache. */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;

        /* Fill the cache from where it left off up to n. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)3;
            } else if (i == 2) {
                val = (seq_t)15;
            } else if (i == 3) {
                val = (seq_t)57;
            } else {
                /* For i >= 4:
                 * a(i) = 3 * a(i-1) + 3 * a(i-2) + 3 * a(i-3)
                 * a(i-1) is at cache[i-2], a(i-2) at cache[i-3], a(i-3) at cache[i-4] */
                val = (cache[i - 2] + cache[i - 3] + cache[i - 4]) * (seq_t)3;
            }
            cache[i - 1] = val;   /* store at 0‑based index */
        }

        cache_len = n;
    }

    return cache[n - 1];
}
