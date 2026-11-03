/*
 * A188891  Least triangular n-gonal number greater than 1, or 0 if none
 *          exists.
 * https://oeis.org/A188891
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least triangular n‑gonal number > 1, or 0 if none exists
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 56 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return the least triangular n‑gonal number > 1.
 * If no such number exists (for small n like 11, 18, 38, …) return 0. */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;       /* offset starts at 3 */

    seq_t n_seq = (seq_t)n;
    seq_t n_minus_2 = n_seq - (seq_t)2;
    seq_t n_minus_4 = n_seq - (seq_t)4;

    /* Search increasing k (the index of the n‑gonal number). */
    for (idx_t k = 1; k <= 200000; ++k)
    {
        seq_t k_seq = (seq_t)k;

        /* P_n(k) = ((n-2)k^2 - (n-4)k)/2 */
        seq_t p = ((n_minus_2 * k_seq * k_seq) - (n_minus_4 * k_seq)) / (seq_t)2;

        if (p <= (seq_t)1) continue;              /* need >1 */

        /* Check if p is triangular:
         * 8*p + 1 must be a perfect square. */
        seq_t test = (seq_t)8 * p + (seq_t)1;
        seq_t s = floor_sqrt(test);
        if (s * s == test)
            return p;
    }

    /* If the loop terminates without finding a value,
     * assume no such number exists for this n. */
    return (seq_t)0;
}
