/*
 * A222090  Number of n X 2 arrays of occupancy after each element stays put
 *          or moves to some horizontal, vertical or antidiagonal neighbor,
 *          without move-in move-out left turns.
 * https://oeis.org/A222090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) - 90*a(n-2) + 217*a(n-3) - 258*a(n-4) +
 *            156*a(n-5) - 44*a(n-6) + 4*a(n-7) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
#  include <new>
#endif

/* Cache of previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;     /* allocated capacity (number of slots) */
static idx_t max_computed = 0;  /* largest index that has been computed */

/* Return the nth term of sequence A222090 (1‑based index) */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1. */
    if (n < 1)
        return 0;

    /* Ensure the cache can hold indices 0 … n. */
    if (n + 1 > cache_cap) {
        idx_t old_cap = cache_cap;
        idx_t new_cap = n + 1;
        seq_t *new_cache;

#if defined(__cplusplus)
        /* C++: allocate via new[] */
        new_cache = new seq_t[new_cap];
        if (cache) {
            /* copy existing elements */
            for (idx_t i = 0; i < old_cap; ++i)
                new_cache[i] = cache[i];
            delete[] cache;
        }
#else
        /* C: allocate via malloc */
        new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)          /* allocation failed */
            return 0;
        if (cache) {
            for (idx_t i = 0; i < old_cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
#endif
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Base values for indices 1 … 7 */
    static const seq_t base[8] = {
        (seq_t)0,          /* unused 0‑th element */
        (seq_t)3,          /* a(1) */
        (seq_t)31,         /* a(2) */
        (seq_t)308,        /* a(3) */
        (seq_t)3034,       /* a(4) */
        (seq_t)29860,      /* a(5) */
        (seq_t)293848,     /* a(6) */
        (seq_t)2891636     /* a(7) */
    };

    /* Compute missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 7)
            cache[i] = base[i];
        else {
            seq_t t = (seq_t)17 * cache[i-1]
                    - (seq_t)90 * cache[i-2]
                    + (seq_t)217 * cache[i-3]
                    - (seq_t)258 * cache[i-4]
                    + (seq_t)156 * cache[i-5]
                    - (seq_t)44  * cache[i-6]
                    + (seq_t)4   * cache[i-7];
            cache[i] = t;
        }
    }

    max_computed = n;
    return cache[n];
}
