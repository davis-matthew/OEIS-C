/*
 * A224434  The hyper-Wiener index of the dendrimer D_3[n], defined
 *          pictorially in the A. R. Ashrafi et al. reference.
 * https://oeis.org/A224434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 107559 - 551400*2^n + 108900*4^n*n^2 + 444198*4^n
 *            -304590*4^n*n + 74910*2^n*n - 18150*2^n*n^2
 * Range:     u8 none, u16 n <= 0, u32 n <= 5, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /* Powers of 2 and 4 */
    seq_t pow2n = big_pow((seq_t)2, N);
    seq_t pow4n = big_pow((seq_t)4, N);

    /* n^2 */
    seq_t n2 = N * N;

    /* Begin with the constant term */
    seq_t result = (seq_t)107559;

    /* Add positive contributions */
    result += (seq_t)444198 * pow4n;
    result += (seq_t)74910 * pow2n * N;
    result += (seq_t)108900 * pow4n * n2;

    /* Subtract negative contributions */
    result -= (seq_t)551400 * pow2n;
    result -= (seq_t)304590 * pow4n * N;
    result -= (seq_t)18150 * pow2n * n2;

    return result;
}
