/*
 * A224428  The hyper-Wiener index of the dendrimer NS[n], defined pictorially
 *          in the A. R. Ashrafi et al. reference.
 * https://oeis.org/A224428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7560 - 64000*4^n*n - 113312*2^n + 106784*4^n +
 *            25600*4^n*n^2
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t N     = (seq_t)n;
    seq_t pow4n = pow4(n);   /* 4^n */
    seq_t pow2n = pow2(n);   /* 2^n */

    seq_t term1 = (seq_t)25600;
    term1 *= pow4n;
    term1 *= N;
    term1 *= N;          /* 25600*4^n*n^2 */

    seq_t term2 = (seq_t)64000;
    term2 *= pow4n;
    term2 *= N;          /* 64000*4^n*n */

    seq_t term3 = (seq_t)113312;
    term3 *= pow2n;       /* 113312*2^n */

    seq_t term4 = (seq_t)106784;
    term4 *= pow4n;       /* 106784*4^n */

    seq_t term5 = (seq_t)7560;

    seq_t sum_pos = term5 + term4 + term1; /* positive terms */
    seq_t sum_neg = term2 + term3;         /* terms to subtract */

    return sum_pos - sum_neg;
}
