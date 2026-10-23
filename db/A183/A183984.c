/*
 * A183984  1/4 the number of (n+1) X 8 binary arrays with all 2 X 2 subblock
 *          sums the same.
 * https://oeis.org/A183984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n ≥ 1 a(n) = 3*2^(n/2-1) + 2^(n-1) + 78 if n is even, a(n) =
 *            2^(n-1) + 2^((n+1)/2) + 78 if n is odd (See OEIS A183984)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term.  For n < 1 the result is
       undefined; return 0 for safety. */
    if (n < 1)
        return 0;

    seq_t pow_n = pow_seq((seq_t)2, n - 1);            /* 2^(n-1) */

    if ((n & 1) == 0) {                               /* even n   */
        seq_t pow_half = pow_seq((seq_t)2, n / 2 - 1); /* 2^(n/2-1) */
        return (seq_t)3 * pow_half + pow_n + (seq_t)78;
    } else {                                          /* odd n    */
        seq_t pow_half = pow_seq((seq_t)2, (n + 1) / 2); /* 2^((n+1)/2) */
        return pow_n + pow_half + (seq_t)78;
    }
}
