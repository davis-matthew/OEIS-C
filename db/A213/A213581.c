/*
 * A213581  Antidiagonal sums of the convolution array A213571.
 * https://oeis.org/A213581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 + (n-2)*2^(n+2) - ((n-2)*n*(n+5))/6
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Base cases for n = 1 … 6 */
    static const seq_t base[6] = {
        (seq_t)1,
        (seq_t)8,
        (seq_t)36,
        (seq_t)124,
        (seq_t)367,
        (seq_t)988
    };
    if (n >= 1 && n <= 6)
        return base[n - 1];

    /* n ≥ 7 */
    seq_t n_s   = (seq_t)n;
    seq_t n_m2  = n_s - (seq_t)2;      /* n – 2 */
    seq_t n_p5  = n_s + (seq_t)5;      /* n + 5 */

    /* (n-2) * 2^(n+2) */
    seq_t power = pow2(n + (idx_t)2);
    seq_t term1 = n_m2 * power;

    /* (n-2) * n * (n+5) / 6 */
    seq_t poly = n_m2 * n_s * n_p5;
    poly /= (seq_t)6;

    /* Final result */
    seq_t result = (seq_t)8 + term1 - poly;
    return result;
}
