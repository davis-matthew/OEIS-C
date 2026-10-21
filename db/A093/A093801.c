/*
 * A093801  a(n) = b(n)*Integral_{x=0..1/(4^n)} (1 - sqrt(x)) dx, where b(n) =
 *          3*24^n.
 * https://oeis.org/A093801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*6^n - 2*3^n * Recurrence: a(0) = 1, a(1) = 12, a(n) =
 *            9*a(n-1) - 18*a(n-2) for n > 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq_t_r
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_t_r.h"

seq_t a(idx_t n) {
    seq_t pow6 = pow_seq_t_r(6, n);
    seq_t pow3 = pow_seq_t_r(3, n);
    return 3 * pow6 - 2 * pow3;
}
