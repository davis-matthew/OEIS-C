/*
 * A047869  Subsets of an 8-element set in order by number of elements in each
 *          subset.
 * https://oeis.org/A047869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 256, u32 n <= 256, u64 n <= 256,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A047869
 *  Subsets of an 8-element set in order by number of elements in each subset.
 *  Sequence values are non‑negative integers < 2^8, represented as binary
 *  vectors.
 *
 *  The implementation precomputes the 256 terms on first call and caches
 *  them.  Subsequent calls return the cached value, ensuring O(1) time
 *  per query after the initial O(256) precomputation.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t */

static seq_t *a_cache = NULL;   /* cached sequence values */
static size_t a_cache_size = 0; /* number of cached terms */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Array size for all subsets of an 8‑element set */
    const size_t TOTAL = 256; /* 2^8 */

    if (a_cache == NULL) {
        /* Allocate and fill the cache */
        a_cache = (seq_t *)malloc(TOTAL * sizeof(seq_t));
        if (!a_cache) return (seq_t)0; /* fallback on allocation failure */

        size_t pos = 0;
        for (int k = 0; k <= 8; ++k) {          /* cardinality loop */
            for (uint16_t mask = 0; mask < TOTAL; ++mask) {
                /* __builtin_popcount is available for unsigned int */
                if (__builtin_popcount((unsigned int)mask) == k) {
                    a_cache[pos++] = (seq_t)mask;
                }
            }
        }
        a_cache_size = TOTAL;
    }

    /* Convert index to 0‑based */
    if (n == 0 || (size_t)n > a_cache_size) {
        return (seq_t)0; /* undefined for out‑of‑range n */
    }
    return a_cache[(size_t)n - 1];
}
