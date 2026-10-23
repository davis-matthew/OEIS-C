/*
 * A136328  a(n) = Wiener index of the odd graph O_n.
 * https://oeis.org/A136328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = (binomial(2*n - 1, n - 1)/2) * * ( Σ_{j=0}^{⌊n/2⌋-1}
 *            ((2*j + 1)*n/(j + 1))*C(n-1, j)^2 * + Σ_{j=⌊n/2⌋}^{n-2}
 *            (2*(n-1-j)*n/(j + 1))*C(n-1, j)^2 )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case for the offset 1 */
    if (n < 2)
        return 0;  /* a(1) = 0 */

    seq_t sum = 0;
    idx_t half = n / 2;           /* floor(n/2) */
    seq_t bin = 1;                /* C(n-1, 0) */

    for (idx_t j = 0; j <= n - 2; ++j) {
        seq_t sq = bin * bin;     /* C(n-1, j)^2 */

        seq_t mult;
        if (j < half) {
            mult = (seq_t)(2 * j + 1) * (seq_t)n;
        } else {
            mult = (seq_t)2 * (seq_t)(n - 1 - j) * (seq_t)n;
        }

        /* term = (mult * sq) / (j + 1) */
        seq_t term = (mult * sq) / (seq_t)(j + 1);
        sum += term;

        /* Prepare bin for next j: C(n-1, j+1) = C(n-1, j) * (n-1-j)/(j+1) */
        if (j < n - 2) {
            seq_t numerator   = bin * (seq_t)(n - 1 - j);
            seq_t denominator = (seq_t)(j + 1);
            bin = numerator / denominator;
        }
    }

    /* base = C(2*n - 1, n - 1) */
    seq_t base = binomial(2 * n - 1, n - 1);

    /* (base * sum) / 2 with minimal intermediate size */
    if (base & 1) {          /* base odd → sum must be even */
        sum >>= 1;           /* sum / 2 */
        return base * sum;
    } else {
        base >>= 1;          /* base / 2 */
        return base * sum;
    }
}
