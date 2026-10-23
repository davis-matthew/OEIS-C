/*
 * A183985  1/4 the number of (n+1) X 9 binary arrays with all 2 X 2 subblock
 *          sums the same.
 * https://oeis.org/A183985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   if (n%2==0) a(n)=3*2^(n/2-1)+2^(n-1)+150; else
 *            a(n)=2^(n-1)+2^((n+1)/2)+150; Uses the closed form derived from
 *            the conjectured explicit formulas
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* behaviour for n < 1 is undefined; return 0 */
        return (seq_t)0;
    }

    /* 2^(n-1) is common to both cases. */
    seq_t t1 = pow2(n - 1);

    if ((n & (idx_t)1) == 0) {            /* even n */
        idx_t half = n / 2;               /* n is even, half >= 1 for n>=2 */
        /* 3 * 2^(n/2 - 1) */
        seq_t t3 = pow2(half - 1);
        return (seq_t)3 * t3 + t1 + (seq_t)150;
    } else {                              /* odd n */
        seq_t t2 = pow2((n + 1) / 2);
        return t1 + t2 + (seq_t)150;
    }
}
