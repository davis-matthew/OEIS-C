/*
 * A085267  Numbers having at least two representations as sum of a squarefree
 *          number and a nonzero square.
 * https://oeis.org/A085267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   ipow_i32, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085267:
   Numbers having at least two representations as sum of a
   squarefree number and a nonzero square.
   Computation: iterate integers, for each count representations
   m = sf + t^2 (t ≥ 1, sf squarefree, sf ≥ 1).  Return the nth
   integer with count ≥ 2. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_squarefree.h"

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

static unsigned int count_reps(seq_t m) {
    seq_t m_minus1 = m - 1;                     /* t² ≤ m-1 ensures sf ≥ 1 */
    idx_t  t_limit = (idx_t)floor_sqrt(m_minus1);
    unsigned int reps = 0;
    for (idx_t t = 1; t <= t_limit; ++t) {
        seq_t sq = (seq_t)t * t;
        seq_t sf = m - sq;
        if (is_squarefree(sf)) {
            ++reps;
            if (reps >= 2) break;              /* early exit */
        }
    }
    return reps;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined, guard */
    idx_t  found = 0;
    seq_t  m = 1;
    for (;;) {
        if (count_reps(m) >= 2) {
            ++found;
            if (found == n)
                return m;
        }
        ++m;
    }
}
