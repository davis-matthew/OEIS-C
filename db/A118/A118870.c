/*
 * A118870  Number of binary sequences of length n with no subsequence 0101.
 * https://oeis.org/A118870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=4, a(3)=8; for n≥4: a(n)=2*a(n-1) - a(n-2)
 *            + 2*a(n-3) - a(n-4)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 69, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#if defined(__cplusplus)
/* Reinterpret_cast works in C++ */
#  define _SEQ_REINTER_CAST(ptr, type) reinterpret_cast<type>(ptr)
#else
/* C-style cast works in C */
#  define _SEQ_REINTER_CAST(ptr, type) ((type)(ptr))
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamic cache of computed terms */
    static size_t cap   = 0;      /* number of cache entries currently stored */

    /* Grow the cache if we need more entries */
    if ((size_t)(n + 1) > cap)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache =
            _SEQ_REINTER_CAST(realloc(cache, new_cap * sizeof(seq_t)), seq_t *);

        cache = new_cache;

        /* Compute newly added entries.  Start with the initial values. */
        for (idx_t i = (idx_t)cap; i <= n; ++i)
        {
            size_t ui = (size_t)i;   /* index into the array */

            if (i == 0)
                cache[ui] = (seq_t)1;
            else if (i == 1)
                cache[ui] = (seq_t)2;
            else if (i == 2)
                cache[ui] = (seq_t)4;
            else if (i == 3)
                cache[ui] = (seq_t)8;
            else
                cache[ui] = (seq_t)(
                    (seq_t)2 * cache[ui - 1] -
                    cache[ui - 2] +
                    (seq_t)2 * cache[ui - 3] -
                    cache[ui - 4]
                );
        }

        cap = new_cap;
    }

    return cache[(size_t)n];
}
