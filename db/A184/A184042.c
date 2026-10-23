/*
 * A184042  1/9 the number of (n+1) X 4 0..2 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { 3*2^(n/2) + 2^(n+1) + 17 , n even * 2^(n+1) +
 *            2^((n+3)/2) + 17 , n odd }
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* n >= 1 ; behaviour for n < 1 unspecified */
    seq_t res;
    if ((n & 1) == 0) { /* even */
        seq_t t1 = pow2(n / 2);
        t1 = mul_small(t1, 3);
        seq_t t2 = pow2(n + 1);
        res = t1 + t2 + (seq_t)17;
    } else { /* odd */
        seq_t t1 = pow2(n + 1);
        seq_t t2 = pow2((n + 3) / 2);
        res = t1 + t2 + (seq_t)17;
    }
    return res;
}
