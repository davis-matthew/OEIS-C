/*
 * A169797  Expansion of ((1-x)/(1-2x))^10.
 * https://oeis.org/A169797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = ((n+11) * (n^8 + 124*n^7 + 5986*n^6 + 143944*n^5
 *            + 1836529*n^4 + 12358156*n^3 + 42005484*n^2 + 64730736*n +
 *            33747840) * 2^(n-17)) / 2835 for n > 0
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nseq = (seq_t)n;

    /* Compute the polynomial via Horner's rule */
    seq_t poly = (seq_t)1;
    poly = poly * nseq + (seq_t)124;
    poly = poly * nseq + (seq_t)5986;
    poly = poly * nseq + (seq_t)143944;
    poly = poly * nseq + (seq_t)1836529;
    poly = poly * nseq + (seq_t)12358156;
    poly = poly * nseq + (seq_t)42005484;
    poly = poly * nseq + (seq_t)64730736;
    poly = poly * nseq + (seq_t)33747840;

    seq_t num = (nseq + (seq_t)11) * poly;

    if (n >= 17) {
        seq_t two = pow2(n - 17);
        num = num * two;
    } else {
        seq_t two = pow2(17 - n);
        num = num / two;
    }

    num = num / (seq_t)2835;
    return num;
}
