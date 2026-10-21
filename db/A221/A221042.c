/*
 * A221042  The Wiener index of the Bethe cactus lattice graph D_n defined
 *          pictorially in the Hosoya - Balasubramanian reference.
 * https://oeis.org/A221042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)+3^n*(3*n+4)+3^(2*n)*(3*n-9/2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    /* Compute 3^n and 9^n = (3^n)^2 */
    seq_t p3 = pow3(n);
    seq_t p9 = p3 * p3;

    /* term1 = 2 * 3^n * (3*n + 4) */
    seq_t temp1  = (n_seq * (seq_t)3) + (seq_t)4;
    seq_t term1  = p3 * temp1;
    term1 *= (seq_t)2;

    /* term2 = 9^n * (6*n - 9) */
    seq_t temp2  = (n_seq * (seq_t)6) - (seq_t)9;
    seq_t term2  = p9 * temp2;

    /* Sum and divide by 2 */
    seq_t sum = (seq_t)1 + term1 + term2;
    return sum / (seq_t)2;
}
