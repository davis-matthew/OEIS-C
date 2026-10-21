/*
 * A224466  The hyper-Wiener index of the nanostar dendrimer defined
 *          pictorially as NS[n] in the Z. S. Irani reference.
 * https://oeis.org/A224466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11038 + 2^n*(7000*n^2 - 8060*n - 3132) + 4^n*(1600*n^2 -
 *            9600*n + 19274)
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;

    // Compute powers of 2: 2^n and 4^n (which is (2^n)^2)
    seq_t pow2n = pow2(n);          // 2^n
    seq_t pow4n = pow2n * pow2n;    // 4^n

    // Quadratic coefficients, all cast to seq_t to avoid unintended overflow
    seq_t coeff1 = (seq_t)7000 * N * N - (seq_t)8060 * N - (seq_t)3132;
    seq_t coeff2 = (seq_t)1600 * N * N - (seq_t)9600 * N + (seq_t)19274;

    // Final formula
    seq_t result = (seq_t)11038 + coeff1 * pow2n + coeff2 * pow4n;
    return result;
}
