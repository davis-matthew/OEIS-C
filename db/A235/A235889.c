/*
 * A235889  Number of (n+1) X (4+1) 0..3 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(210 + 45*2^n + 4^n) (n ≥ 1)
 * Range:     u8 none, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* For n ≥ 1 a(n) = 4*(210 + 45*2^n + 4^n) */
    if (n == 0)    /* offset is 1; define a(0)=0 for safety */
        return 0;

    seq_t two_n  = pow2(n);   /* 2^n */
    seq_t four_n = pow4(n);   /* 4^n */

    seq_t val = 210;          /* constant part */
    val = val + 45 * two_n;   /* add 45*2^n */
    val = val + four_n;       /* add 4^n */

    return val * 4;           /* final multiplication by 4 */
}
