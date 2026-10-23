/*
 * A184035  1/16 the number of (n+1) X 6 0..3 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(n/2-1)+9*2^(n-1)+154 for n even; a(n) =
 *            9*2^(n-1)+3*2^((n+1)/2)+154 for n odd, n≥1
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined offset; return 0 */

    seq_t res;
    if ((n & 1) == 0) { /* even n */
        seq_t t1 = seq_pow((seq_t)2, n - 1);          /* 2^(n-1) */
        seq_t t2 = seq_pow((seq_t)2, (n / 2) - 1);    /* 2^(n/2-1) */
        res = (seq_t)9 * t2 + (seq_t)9 * t1 + (seq_t)154;
    } else { /* odd n */
        seq_t t1 = seq_pow((seq_t)2, n - 1);          /* 2^(n-1) */
        seq_t t2 = seq_pow((seq_t)2, (n + 1) / 2);    /* 2^((n+1)/2) */
        res = (seq_t)9 * t1 + (seq_t)3 * t2 + (seq_t)154;
    }
    return res;
}
