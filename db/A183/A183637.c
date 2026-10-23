/*
 * A183637  Number of (n+1) X 5 0..3 arrays with every 2 X 2 subblock summing
 *          to 6.
 * https://oeis.org/A183637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 570 + 45*2^(3+n) + 10*3^(2+n) + 4^(1+n)
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence offset: a(1) = 1576 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* If n is 0 or negative (should not occur), return 0. */
    if (n == 0) return 0;

    seq_t res = 0;

    /* add the constant part */
    res += (seq_t)570;

    /* add the 2^(3+n) term multiplied by 45 */
    res += (seq_t)45 * pow2(3 + n);

    /* add the 3^(2+n) term multiplied by 10 */
    res += (seq_t)10 * pow3(2 + n);

    /* add the 4^(1+n) term */
    res += pow4(1 + n);

    return res;
}
