/*
 * A239609  Sin(arcsin(n)- 3 arccos(n))^2.
 * https://oeis.org/A239609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A144130(n)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 152, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* coefficients of the polynomial in the generating function
       (indices 0..8 correspond to powers x^0 .. x^8) */
    static const int coeffs[9] = {
        1,   -8,     9436,   248200,
        1032646, 1032520, 248284, 9400, 1
    };

    seq_t pos = 0;   /* positive contributions   */
    seq_t neg = 0;   /* absolute value of negative  */

    for (int k = 0; k < 9; ++k) {
        seq_t N = (seq_t)n + (8 - k);   /* argument of binomial */
        if (N < 8) continue;           /* binom(N,8) = 0 for N < 8 */

        /* compute binom(N,8) = product_{i=1..8} (N-8+i) / i   */
        seq_t bin = 1;
        for (int i = 1; i <= 8; ++i) {
            bin = bin * (N - (8 - i)) / i;
        }

        int c = coeffs[k];
        seq_t add = bin * (seq_t)(c >= 0 ? c : -c);  /* absolute value */

        if (c >= 0)
            pos += add;
        else
            neg += add;
    }

    return pos - neg;
}
