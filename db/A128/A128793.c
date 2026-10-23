/*
 * A128793  a(n) = n^3*7^n.
 * https://oeis.org/A128793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * 7^n (offset 0, a(0)=0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    /* convert n to seq_t to keep all arithmetic in the target type */
    seq_t nseq = (seq_t)n;
    /* compute n^3  */
    seq_t n3 = nseq * nseq * nseq;
    /* compute 7^n  */
    seq_t pow7 = big_pow((seq_t)7, nseq);
    /* final result  */
    return n3 * pow7;
}
