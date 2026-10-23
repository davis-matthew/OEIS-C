/*
 * A189391  The minimum possible value for the apex of a triangle of numbers
 *          whose base consists of a permutation of the numbers 0 to n, and
 *          each number in a higher row is the sum of the two numbers directly
 *          below it.
 * https://oeis.org/A189391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A189391: Minimum possible apex of a triangular number puzzle
   Formula:
       a(0) = 0
       for n even:  a(n) = ((2*n+1)/2) * binom(n, n/2) - 2^(n-1)
       for n odd : a(n) = ((n+1)/2)   * binom(n+1, (n+1)/2) - 2^(n-1)
   (binom returns the binomial coefficient). */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t pow_val = pow2(n - 1);               /* 2^(n-1) */

    if ((n & 1) == 0) {                       /* n even */
        /* compute ((2*n+1)/2) * C(n,n/2) safely */
        seq_t bin = binom(n, n / 2);           /* C(n,n/2) */
        seq_t bin_half = bin / 2;              /* bin is even for even n */
        seq_t prod = bin_half * ((seq_t)(2 * n + 1));
        return prod - pow_val;
    } else {                                   /* n odd */
        /* compute ((n+1)/2) * C(n+1,(n+1)/2) */
        seq_t bin = binom(n + 1, (n + 1) / 2);
        seq_t left = (seq_t)(n + 1) / 2;
        seq_t prod = left * bin;
        return prod - pow_val;
    }
}
