/*
 * A163581  Number of zeros of sin(x) in integer intervals starting with
 *          (0,1).
 * https://oeis.org/A163581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if ∃ m>0 such that n < m·π < n+1; 0 otherwise It is
 *            equivalent to a(n)=1 iff n = ⌊m·π⌋ for some m∈ℕ⁺ We generate all
 *            floor(m·π) for m up to the largest needed, using the rational
 *            approximation 355/113 of π, which is accurate enough for all
 *            m≤10⁴ (error <0.5/m). The cache stores 0/1 values
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   free_cache, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* static cache of 0/1 values, indexed by n */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_m = 0;     /* last m processed */

    if (n >= cache_size) {
        /* enlarge cache to hold indices up to n */
        idx_t new_size = n + 1;
        seq_t *new_cache = seq_alloc_array(new_size);
        if (cache != NULL) {
            free_cache(cache);
        }
        cache = new_cache;
        cache_size = new_size;

        /* compute floor(m·π) for m = last_m+1, … while the result < cache_size */
        for (idx_t m = last_m + 1;; ++m) {
            /* evaluate ⌊m·π⌋ ≈ ⌊m·355/113⌋; this is exact for all m≤10⁴ */
            idx_t k = (m * (idx_t)355ULL) / (idx_t)113ULL;
            if (k >= cache_size) {
                last_m = m - 1;
                break;
            }
            cache[k] = (seq_t)1;
        }
    }
    return cache[n];
}
