/*
 * A011573  Stirling numbers of second kind S2(24,n).
 * https://oeis.org/A011573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = S(24,n) (Stirling numbers of the second kind)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Given the fixed n = 24, the recurrence S(n,k)=S(n-1,k-1)+k·S(n-1,k) */
/* computes all values for 1 ≤ k ≤ 24.  A one‑time initialization keeps them memoized. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t x, unsigned int m) */

/* Cache for S(24,k) where k ranges from 0..24.  cache[0] is unused for k≥1. */
static seq_t cache[25] = {0};
static bool initialized = false;

/* Computes the table S(0,0)=1 and S(24,k) for 1≤k≤24. */
static void init_cache(void)
{
    if (initialized) return;

    cache[0] = 1;            /* S(0,0) = 1 */

    for (idx_t n = 1; n <= 24; ++n) {
        /* Update array from high to low to use the previous row in-place. */
        for (idx_t k = n; k >= 1; --k) {
            /* cache[k] holds S(n-1,k) and cache[k-1] holds S(n-1,k-1).  */
            seq_t term = cache[(size_t)k - 1];
            seq_t mult = mul_small(cache[(size_t)k], (unsigned int)k);
            cache[(size_t)k] = term + mult;
        }
        cache[0] = 0;        /* S(n,0) = 0 for n>0 */
    }
    initialized = true;
}

/* Public entry point defined by the OEIS requirement. */
seq_t a(idx_t n)
{
    init_cache();

    if (n == 0 || n > 24) return 0;
    return cache[(size_t)n];
}
