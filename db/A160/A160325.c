/*
 * A160325  Number of ways to express n=0,1,2,... as the sum of a triangular
 *          number, an even square and a pentagonal number.
 * https://oeis.org/A160325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{<x,y,z>: x,y,z=0,1,2,… & x(x+1)/2 + 4y² + (3z²−z)/2 =
 *            n}|
 * Range:     u8 n <= 4, u16 n <= 508, u32 n <= 9504, u64 n <= 9504,
 *            u128 n <= 9504, big any n
 * Helpers:   _isqrt_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_isqrt_local.h"

seq_t a(idx_t n)
{
    seq_t count = (seq_t)0;
    seq_t n_seq = (seq_t)n;

    for (idx_t x = 0;; ++x) {
        seq_t tri = (seq_t)x * ((seq_t)x + (seq_t)1) / (seq_t)2;
        if (tri > n_seq)
            break;

        for (idx_t y = 0;; ++y) {
            seq_t ysq = (seq_t)y * (seq_t)y;
            seq_t sum = tri + (seq_t)4 * ysq;
            if (sum > n_seq)
                break;

            seq_t rem = n_seq - sum;

            /* remainder zero ⇒ z = 0 is a valid pentagonal */
            if (rem == (seq_t)0) {
                ++count;
                continue;
            }

            /* Check if rem is a pentagonal number */
            seq_t D = (seq_t)24 * rem + (seq_t)1;
            seq_t sqrtD = _isqrt_local(D);
            if (sqrtD * sqrtD != D)
                continue;
            /* z must be integer: (sqrtD + 1) divisible by 6 */
            if (((sqrtD + (seq_t)1) % (seq_t)6) == (seq_t)0)
                ++count;
        }
    }

    return count;
}
