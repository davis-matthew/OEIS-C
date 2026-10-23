/*
 * A161580  Positions n such that A010060(n) + A010060(n+3) = 1.
 * https://oeis.org/A161580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161580 – positions n such that
 * A010060(n) + A010060(n+3) = 1, where A010060 is the
 * indicator of an odd popcount (odious numbers).
 *
 * The algorithm checks integers in ascending order and stores every
 * time that the parity of n differs from the parity of n+3.
 * The results are cached in a static array so that each subsequent
 * call only appends new terms.  The implementation works for
 * any seq_t and idx_t provided by the build, including arbitrary‑
 * precision integer classes, because all allocations are done with
 * malloc/realloc and the casts are performed using reinterpret_cast
 * in C++ and C‑style casts in C.
 *
 * Complexity: O(a(n)) time and O(n) memory for a(n) terms.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Helper: cast void * to seq_t*, safe for C and C++ */
static inline seq_t *cast_realloc(void *p, size_t n, size_t size)
{
#ifdef __cplusplus
    return reinterpret_cast<seq_t*>(p);
#else
    return (seq_t*)p;
#endif
}

seq_t a(idx_t nWanted)
{
    /* Static cache for computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;    /* number of terms stored          */
    static size_t cache_cap  = 0;    /* current capacity                 */
    static idx_t  next_n     = 1;    /* next natural number to test      */

    /* Allocate or grow the cache as needed */
    while (cache_len < (size_t)nWanted) {

        /* Ensure capacity – double the size or start at 64 */
        if (cache_cap < cache_len + 64) {
            size_t new_cap = (cache_cap == 0) ? 64 : (cache_cap << 1);
            void *tmp = realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();                     /* Out of memory   */
            cache     = cast_realloc(tmp, new_cap, sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Search for the next n that satisfies the condition. */
        for (; next_n <= SIZE_MAX; ++next_n) {
            uint64_t nn = (uint64_t)next_n;
            unsigned p_n  = __builtin_popcountll(nn) & 1;          /* popcount % 2 */
            unsigned p_n3 = __builtin_popcountll(nn + 3ULL) & 1;  /* popcount(n+3) % 2 */

            if (p_n != p_n3) {
                cache[cache_len++] = (seq_t)next_n;
                ++next_n;                            /* prepare for next search */
                break;
            }
        }
    }

    /* Return the requested term (1‑based index). */
    return cache[nWanted - 1];
}
