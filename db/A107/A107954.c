/*
 * A107954  Number of chains in the power set lattice, or the number of fuzzy
 *          subsets of an (n+4)-element set X_(n+4) with specification n
 *          elements of one kind, 3 elements of another and 1 of yet another
 *          kind.
 * https://oeis.org/A107954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 2^(n+1)*( (n^4 + 23*n^3)/6 + (79*n^2 + 185*n)/3 + 40 ) -
 *            1
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t ns   = (seq_t)n;
    seq_t ns2  = ns * ns;                     /* n^2 */
    seq_t ns3  = ns2 * ns;                    /* n^3 */
    seq_t ns4  = ns2 * ns2;                   /* n^4 */

    /* N = n^4 + 23*n^3 + 158*n^2 + 370*n + 240  (numerator of the inner rational) */
    seq_t N = ns4
            + (seq_t)23 * ns3
            + (seq_t)158 * ns2
            + (seq_t)370 * ns
            + (seq_t)240;

    /* 2^n * N / 3 - 1 is equivalent to the original formula */
    seq_t pow2_n = pow2(n);                   /* 2^n  */
    seq_t prod   = pow2_n * N;                /* 2^n * N   */
    seq_t res    = prod / (seq_t)3 - (seq_t)1;/* final result */

    return res;
}
