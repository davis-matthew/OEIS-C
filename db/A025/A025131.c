/*
 * A025131  3rd elementary symmetric function of C(n,0), C(n,1), ..., C(n,n).
 * https://oeis.org/A025131
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( (2^n)^3 - 3 * 2^n * C(2n,n) + 2 * ∑_{k=0}^n C(n,k)^3 )
 *            / 6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* p1 = 2^n */
    seq_t p1 = 1;
    for (idx_t i = 0; i < n; ++i)
        p1 *= 2;

    /* p2 = C(2n, n) */
    seq_t p2 = binom(2*n, n);

    /* p3 = Σ C(n,k)^3 */
    seq_t p3 = 0;
    seq_t binom_val = 1;          /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t cube = binom_val * binom_val;
        cube *= binom_val;
        p3 += cube;

        if (k < n)
        {
            binom_val = binom_val * ((seq_t)(n - k)) / ((seq_t)(k + 1));
        }
    }

    /* numerator = p1^3 - 3*p1*p2 + 2*p3 */
    seq_t numerator = p1 * p1;
    numerator *= p1;             /* p1^3 */

    seq_t temp = p1 * p2;
    temp *= 3;                   /* 3 * p1 * p2 */
    numerator -= temp;

    temp = p3 * 2;               /* 2 * p3 */
    numerator += temp;

    /* result = numerator / 6 */
    return numerator / 6;
}
