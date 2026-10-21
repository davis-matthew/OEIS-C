/*
 * A066356  Numerator of sequence defined by recursion c(n) = 1 + c(n-2) /
 *          c(n-1), c(0) = 0, c(1) = 1.
 * https://oeis.org/A066356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the numerator of the rational sequence c(n) defined by
 *            c(0)=0, c(1)=1, and for n≥2, c(n) = 1 + c(n-2)/c(n-1). The
 *            sequence values are the numerators of the reduced fractions
 *            c(n). This implementation computes a(n) by iterating the
 *            recurrence and reducing the fraction at each step using a...
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   compute_gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compute_gcd_seq.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* p0/q0 = c(n-2), p1/q1 = c(n-1)  */
    seq_t p0 = (seq_t)0, q0 = (seq_t)1; /* c(0) */
    seq_t p1 = (seq_t)1, q1 = (seq_t)1; /* c(1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* t_num = c(n-2).num * c(n-1).den
           t_den = c(n-2).den * c(n-1).num
           c(n) = (t_den + t_num) / t_den   (reduced) */
        seq_t t_num = p0 * q1;
        seq_t t_den = q0 * p1;
        seq_t num = t_den + t_num;
        seq_t den = t_den;

        seq_t g = compute_gcd_seq(num, den);
        seq_t p2 = num / g;
        seq_t q2 = den / g;

        /* shift for next iteration */
        p0 = p1; q0 = q1;
        p1 = p2; q1 = q2;
    }
    return p1;
}
