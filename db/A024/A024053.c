/*
 * A024053  a(n) = 5^n - n^4.
 * https://oeis.org/A024053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - n^4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    /* compute 5^n */
    seq_t p5 = big_pow((seq_t)5, nseq);
    /* compute n^4 */
    seq_t n4 = nseq * nseq * nseq * nseq;
    return p5 - n4;
}
