/*
 * A209725  1/4 the number of (n+1) X 7 0..2 arrays with every 2 X 2 subblock
 *          having distinct clockwise edge differences.
 * https://oeis.org/A209725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(n/2-1)+10 for even n, a(n) = 2^((n+1)/2)+10 for odd
 *            n
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 210, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 as per OEIS offset */
    if ((n & 1U) == 0) {          /* even */
        idx_t exp = n / 2;        /* n/2 ≥ 1 */
        seq_t val = pow2(exp - 1);/* 2^(n/2-1) */
        val *= (seq_t)3;          /* *3 */
        val += (seq_t)10;         /* +10 */
        return val;
    } else {                       /* odd */
        idx_t exp = (n + 1) / 2;
        seq_t val = pow2(exp);    /* 2^((n+1)/2) */
        val += (seq_t)10;         /* +10 */
        return val;
    }
}
