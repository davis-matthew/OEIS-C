/*
 * A139093  Expansion of phi(q) * phi(-q^2) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A139093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{a^2 + 2*b^2 = n} (-1)^b * (1 if a^2 = 0, 2 if a^2 >
 *            0 and a^2 is a perfect square, otherwise 0)
 * Range:     i8 n <= 63, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t n_seq = (seq_t)n;

    /* b = 0 term */
    seq_t s0 = floor_sqrt(n_seq);
    if (s0 * s0 == n_seq) {
        if (s0 == 0)
            res += 1;
        else
            res += 2;
    }

    /* loop over positive b */
    idx_t maxb = (idx_t)floor_sqrt(n_seq / 2);
    for (idx_t b = 1; b <= maxb; ++b) {
        seq_t bb = (seq_t)b;
        seq_t m = n_seq - (seq_t)2 * bb * bb;   /* m = n - 2*b^2 (non‑negative) */
        seq_t ss = floor_sqrt(m);
        if (ss * ss == m) {
            /* base contribution: 2*(-1)^b */
            seq_t contrib = ((b & 1) == 0) ? 2 : -2;
            if (ss != 0)            /* if a^2 > 0, double the count of solutions for a */
                contrib *= 2;       /* gives 4*(-1)^b for positive a^2 */
            res += contrib;
        }
    }

    return res;
}
