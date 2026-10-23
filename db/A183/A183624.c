/*
 * A183624  Number of (n+1) X 2 0..2 arrays with every 2 x 2 subblock summing
 *          to 4.
 * https://oeis.org/A183624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + 2^(2+n) + 3^(1+n)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t res = (seq_t)2;
    res += pow2(n + 2);
    res += pow3(n + 1);
    return res;
}
