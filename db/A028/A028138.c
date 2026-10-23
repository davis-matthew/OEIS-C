/*
 * A028138  Expansion of 1/((1-4x)(1-6x)(1-8x)(1-12x)).
 * https://oeis.org/A028138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*12^n - 2*8^(n+1) + 9*6^n - 4^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t n1seq = nseq + 1;

    seq_t pow12 = big_pow((seq_t)12, nseq);
    seq_t pow6  = big_pow((seq_t)6,  nseq);
    seq_t pow8  = big_pow((seq_t)8,  n1seq);
    seq_t pow4  = big_pow((seq_t)4,  nseq);

    seq_t term1 = (seq_t)9 * pow12;
    seq_t term3 = (seq_t)9 * pow6;
    seq_t term2 = (seq_t)2 * pow8;
    seq_t term4 = pow4;  /* 4^n */

    seq_t sum1 = term1 + term3;
    seq_t sum2 = term2 + term4;

    seq_t res = sum1 - sum2;
    return res;
}
