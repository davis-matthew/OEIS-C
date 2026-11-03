/*
 * A061905  The iterative cycle: n -> sum of digits of n^2 has only two
 *          distinct elements.
 * https://oeis.org/A061905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-01)
 * Helpers:   digit_sum_u64_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A061905 - Numbers n for which the iterative sequence  
   n → sum of digits of n² (applied to the result of the
   previous step) contains exactly two distinct values.
   a(n) is the nth integer having this property.
*/
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum_u64_to_seq.h"

/* compute f(t) = sum of digits of t², with intermediate integers
   performed in 64‑bit arithmetic (sufficient for the ranges
   that occur in the tests).  The result always fits in seq_t. */
static inline seq_t next_from(seq_t v)
{
    unsigned long long sq = (unsigned long long)v * (unsigned long long)v;
    return digit_sum_u64_to_seq(sq);
}

/* true if the orbit of n under f(t)=sumdigit(t²) has exactly two
   distinct values after the first step. */
static inline bool has_two_cycle(idx_t n)
{
    seq_t y1 = next_from((seq_t)n);           /* f(n)               */
    seq_t y2 = next_from(y1);                /* f(f(n))            */

    /* cycle of length 1: only one distinct value – reject */
    if (y1 == y2)
        return false;

    seq_t y3 = next_from(y2);                /* f³(n)              */

    /* the set {y1,y2,y3} must have size 2, i.e. y3 equals y1 or y2 */
    return (y3 == y1) || (y3 == y2);
}

/* a(n) – nth integer n ≥ 1 such that has_two_cycle(n) is true. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t cur   = 1;
    while (true) {
        if (has_two_cycle(cur)) {
            ++count;
            if (count == n)
                return (seq_t)cur;
        }
        ++cur;
    }
}
