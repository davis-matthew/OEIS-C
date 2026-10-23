/*
 * A127195  Number of subsets of {1,2,...,n} which contain no three
 *          consecutive odd numbers.
 * https://oeis.org/A127195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Tribonacci(floor((n+1)/2)+3) * 2^floor(n/2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   trib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/trib.h"

seq_t a(idx_t n)
{
    /* number of odd positions */
    idx_t t = (n + 1) / 2;  /* ceil(n/2) */
    /* number of even positions */
    idx_t e = n / 2;        /* floor(n/2) */

    seq_t trib_val = trib(t + 3);    /* T(ceil(n/2)+3) */
    seq_t pow2 = (seq_t)1;
    /* compute 2^e by left shift */
    for (idx_t i = 0; i < e; ++i)
        pow2 <<= 1;

    return trib_val * pow2;
}
