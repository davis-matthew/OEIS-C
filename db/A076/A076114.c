/*
 * A076114  a(n) = start of the smallest string of n consecutive positive
 *          integers with a square sum, or 0 if no such number exists.
 * https://oeis.org/A076114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 7729, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A076114: a(n) = smallest k such that
 *      n*(2*k + n-1)/2  is a perfect square,
 *      or 0 if no such k exists.
 *
 * Equivalent: a(n) = 0 iff n has the form 4^e * m with e>0 and m odd.
 *
 * The implementation follows the algebraic manipulation:
 *   2*k = (2*r^2)/n - n + 1  where r^2 = sum.
 * We search r from ceil(sqrt(n(n+1)/2)) upwards until the
 * divisibility and parity conditions hold.  All arithmetic is
 * performed in seq_t, as required by the interface.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_seq.h"   /* provides static seq_t isqrt_seq(seq_t) */

seq_t a(idx_t n)
{
    /* Undefined for indices below the offset; return 0. */
    if (n < 1) return 0;

    /* Determine whether n is of the forbidden form 4^e * m
     * with e>0 and m odd.  This is equivalent to v2(n) being
     * a positive even integer. */
    idx_t tmp = n;
    int  v2   = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++v2;
    }
    if (v2 > 0 && (v2 & 1) == 0) return 0;

    /* Common lower bound for r: ceil(sqrt(n*(n+1)/2)). */
    seq_t n_seq      = (seq_t)n;
    seq_t min_sum    = n_seq * (n_seq + 1) / 2;
    seq_t r          = isqrt_seq(min_sum);
    if (r * r < min_sum) ++r;           /* adjustment to ceil */

    /* Search for the first admissible r.  For every r the
     * candidate k is ((2*r^2)/n - n + 1)/2.  We test:
     *   1) n divides 2*r^2,
     *   2) the derived k is integer and >=1. */
    for (;;) {
        seq_t r_sq   = r * r;           /* r^2 in seq_t range */
        seq_t two_r2 = r_sq << 1;       /* 2*r^2 */

        if (two_r2 % n_seq == 0) {
            seq_t div    = two_r2 / n_seq;          /* 2*r^2 / n */
            seq_t temp   = div - n_seq + 1;         /* (2*r^2 / n) - n + 1 */
            if ((temp & 1) == 0) {                 /* parity check */
                seq_t k = temp >> 1;
                if (k >= 1) return k;
            }
        }
        ++r;
    }
}
