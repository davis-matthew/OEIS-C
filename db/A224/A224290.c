/*
 * A224290  Number of permutations of length n containing exactly 3
 *          occurrences of 123 and 3 occurrences of 132.
 * https://oeis.org/A224290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/9)*2^(n-15) * (307008 - 247512*n + 78118*n^2 -
 *            12087*n^3 + 937*n^4 - 33*n^5 + n^6) for n>6
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 23, u64 n <= 49, u128 n <= 106,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 7) {
        /* first few terms from the definition */
        switch ((idx_t)n) {
            case 0: case 1: case 2: case 3: case 4: return 0;
            case 5: return 1;
            case 6: return 6;
            default: break; /* not reached, but keep compiler happy */
        }
    }

    /* compute the polynomial part */
    seq_t nseq = (seq_t)n;
    seq_t poly = (seq_t)307008
               - (seq_t)247512 * nseq
               + (seq_t)78118 * nseq * nseq
               - (seq_t)12087 * nseq * nseq * nseq
               + (seq_t)937  * nseq * nseq * nseq * nseq
               - (seq_t)33   * nseq * nseq * nseq * nseq * nseq
               + nseq * nseq * nseq * nseq * nseq * nseq;
    seq_t poly_div9 = poly / 9;   /* division is exact */

    if (n < 15) {
        /* 2^(n-15) is a fraction: divide by 2^(15-n) */
        uint64_t shift = 15 - (uint64_t)n;   /* shift <= 8 for n>=7 */
        return poly_div9 >> shift;
    } else {
        seq_t pow2 = big_pow((seq_t)2, (seq_t)(n - 15));
        return poly_div9 * pow2;
    }
}
