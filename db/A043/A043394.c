/*
 * A043394  Numbers whose base-7 representation contains exactly two 0's.
 * https://oeis.org/A043394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: return the nth positive integer whose base‑7 representation
   contains exactly two digits 0. */

#include <stdlib.h>   /* for malloc, realloc, abort */

/* Helper macro for a type‑safe reallocation that works in C and
   C++ (reinterpret_cast in C++ and a plain cast in C). */
#if defined(__cplusplus)
#  define REINTERPRET_CAST(T, expr) reinterpret_cast<T>(expr)
#else
#  define REINTERPRET_CAST(T, expr) ((T)(expr))
#endif

seq_t a(idx_t n)
{
    /* Static cache for previous results */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached terms */
    static size_t cache_cap = 0;   /* capacity of the cache array   */
    static seq_t last_value = 0;   /* last integer examined          */

    /* Ensure we have at least n terms cached */
    while (cache_len < n)
    {
        /* Next integer to test */
        ++last_value;

        /* Count zeros in base‑7 representation of last_value */
        seq_t tmp = last_value;
        unsigned zeros = 0;
        do
        {
            if ((tmp % 7) == 0)
                ++zeros;
            tmp /= 7;
        } while (tmp != 0 && zeros <= 2);  /* early exit if >2 */

        if (zeros == 2)
        {
            /* Resize cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = REINTERPRET_CAST(seq_t *,
                                                     realloc(cache,
                                                             new_cap * sizeof(seq_t)));
                if (!new_cache) abort();  /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_value;
        }
    }

    return cache[n - 1];
}
