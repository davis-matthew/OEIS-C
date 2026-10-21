/*
 * A220104  k appears k*(k+1) times.
 * https://oeis.org/A220104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that k*(k+1)*(k+2)/3 ≥ n
 * Range:     u8 n <= 1100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offest: 1 (so a(1) is the first term).               */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/sum_runs.h"

seq_t a(idx_t n)
{
    /* Special case: n == 1 gives 1, handled naturally by the search below. */
    idx_t lo = 1;
    idx_t hi = 1;

    /* Find an upper bound hi with sum_runs(hi) ≥ n. */
    while (sum_runs(hi) < (seq_t)n)
        hi <<= 1;          /* double until large enough */

    /* Binary search for the minimal k. */
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (sum_runs(mid) >= (seq_t)n)
            hi = mid;
        else
            lo = mid + 1;
    }

    return (seq_t)lo;
}
