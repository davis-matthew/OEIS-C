/*
 * A020527  2nd Bernoulli polynomial evaluated at powers of 2 (multiplied by
 *          6).
 * https://oeis.org/A020527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(4^n - 2^n) + 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^n and 4^n (= 2^(2n)) using the provided pow2 helper */
    seq_t p2 = pow2(n);
    seq_t p4 = pow2(2 * n);

    /* apply the closed‑form formula */
    seq_t diff = p4 - p2;           /* 4^n - 2^n */
    seq_t res  = diff * (seq_t)6 + (seq_t)1;

    return res;
}
