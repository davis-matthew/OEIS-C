/*
 * A159229  1/16 the number of n X n arrays of squares of integers with every
 *          2X2 subblock summing to 30.
 * https://oeis.org/A159229
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for OEIS A159229:
 * a(n) =  2^((3n-4)/2) + 2^n - 3           for even n
 * a(n) =  6·2^((3n-9)/2) + 2^n - 3        for odd n
 * (n ≥ 2; values for n < 2 are undefined)
 */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Undefined for indices below the offset; return 0 for consistency */
    if (n < 2)
        return (seq_t)0;

    seq_t t1;
    idx_t exp1;

    if ((n & 1) == 0) {            /* even n */
        exp1 = (3 * n - 4) / 2;
        t1 = pow2(exp1);
    } else {                       /* odd n */
        exp1 = (3 * n - 9) / 2;
        t1 = 6 * pow2(exp1);
    }

    seq_t t2 = pow2(n);

    return t1 + t2 - (seq_t)3;
}
