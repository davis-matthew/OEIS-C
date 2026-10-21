/*
 * A238366  a(n) = 5*a(n-2) + 2, a(0) = 1, a(1) = 2.
 * https://oeis.org/A238366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-2) + 2, a(0) = 1, a(1) = 2
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
    #define OBJ_PTR(x) reinterpret_cast<seq_t*>(x)
#else
    #define OBJ_PTR(x) ((seq_t*)(x))
#endif

/* cache of previously computed terms */
static seq_t *cache = NULL;   /* pointer to array of seq_t */
static size_t  cap   = 0;     /* current number of allocated entries */

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Convert the index to a size_t for array addressing */
    size_t N = (size_t)n;

    /* Ensure the cache is large enough for a(N)                                    */
    if (N + 1 > cap) {
        size_t old_cap = cap;
        size_t new_cap = N + 1;

        seq_t *new_cache = OBJ_PTR(realloc(cache, new_cap * sizeof(seq_t)));
        if (!new_cache) {
            /* Allocation failure – return zero (undefined behaviour otherwise) */
            if (cache) free(cache);
            cache = NULL;
            cap   = 0;
            return (seq_t)0;
        }
        cache = new_cache;
        /* Initialise new entries (including the two base cases if they are newly
           inserted).  For i >= 2 use the recurrence a(i) = 5*a(i-2)+2.            */
        for (size_t i = old_cap; i <= N; ++i) {
            if (i == 0)        cache[i] = (seq_t)1;
            else if (i == 1)   cache[i] = (seq_t)2;
            else               cache[i] = (seq_t)5 * cache[i-2] + (seq_t)2;
        }
        cap = new_cap;
    }

    /* The value has now been computed and stored in the cache.  Return it. */
    return cache[N];
}
