/*
 * A224453  The hyper-Wiener index of the nanostar dendrimer NS_2[n], defined
 *          pictorially in the A. R. Ashrafi et al. reference.
 * https://oeis.org/A224453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -6011 + (-36208 - 9720*n + 2800*n^2)*2^n * + (42696
 *            -22400*n + 6400*n^2)*4^n
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Compute powers of 2 and 4 */
    seq_t pow2_n = pow2(n);
    seq_t pow4_n = pow4(n);

    /* Coefficients calculated in signed 64‑bit to handle negativity */
    long long nn  = (long long)n;
    long long coeff1 = 42696 - 22400*nn + 6400*nn*nn;   /* always non‑negative */
    long long coeff2 = -36208 - 9720*nn + 2800*nn*nn;  /* can be negative */

    /* term1 = coeff1 * 4^n (always positive) */
    seq_t term1 = (seq_t)coeff1 * pow4_n;

    /* term2 magnitude times 2^n */
    seq_t abscoeff2 = (coeff2 < 0 ? -(seq_t)coeff2 : (seq_t)coeff2);
    seq_t term2 = abscoeff2 * pow2_n;

    /* combine term1 and term2 respecting the sign of coeff2 */
    seq_t sum;
    if (coeff2 < 0)
        sum = term1 - term2;   /* coeff2 negative */
    else
        sum = term1 + term2;   /* coeff2 non‑negative */

    /* final subtraction of 6011 (guaranteed to not underflow) */
    seq_t result = sum - 6011;

    return result;
}
