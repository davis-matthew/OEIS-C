/*
 * A080754  a(n) = ceiling(n*(1+sqrt(2))).
 * https://oeis.org/A080754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*(1+sqrt(2)))
 * Range:     u8 n <= 5, u16 n <= 60, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t t  = nn * nn;            /* n² */
    seq_t t2 = t * (seq_t)2;       /* 2·n² = (n·sqrt2)² */
    seq_t s  = floor_sqrt(t2);     /* floor(n·sqrt2) */
    return nn + s + (seq_t)1;      /* ceil(n*(1+sqrt2)) = n + floor(n·sqrt2) + 1 */
}
