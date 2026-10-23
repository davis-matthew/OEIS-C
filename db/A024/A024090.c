/*
 * A024090  a(n) = 8^n-n^2.
 * https://oeis.org/A024090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - n^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* 8^n */
    seq_t pow8 = pow_int((seq_t)8, n);

    /* n^2  (cast first to seq_t to avoid overflow in idx_t) */
    seq_t nsq = (seq_t)n * (seq_t)n;

    return pow8 - nsq;
}
