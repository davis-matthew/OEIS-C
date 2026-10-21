/*
 * A227209  Expansion of 1/((1-x)^2*(1-2*x)*(1-4*x)).
 * https://oeis.org/A227209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^(n+3) - 9*2^(n+3) + 6*n + 26)/18
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* compute 4^(n+3) and 2^(n+3) using the utility functions */
    seq_t pow4_val = pow4(n + 3);     /* 4^(n+3) */
    seq_t pow2_val = pow2(n + 3);     /* 2^(n+3) */

    /* numerator: 4^(n+3) - 9*2^(n+3) + 6*n + 26 */
    seq_t num = pow4_val
                - (seq_t)9 * pow2_val
                + (seq_t)6 * (seq_t)n
                + (seq_t)26;

    /* division by 18 (exact integer division) */
    return num / (seq_t)18;
}
