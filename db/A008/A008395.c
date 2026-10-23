/*
 * A008395  Coordination sequence for A_10 lattice.
 * https://oeis.org/A008395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0}^{min(10,n)} coeff[j] * binomial(n-j+9, 9) where
 *            coeff = {1,100,2025,14400,44100,63504,44100,14400,2025,100,1}
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 149, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for integer types, if needed */

static seq_t binomial_nk(idx_t N, int k)
{
    seq_t res = (seq_t)1;
    seq_t n_seq = (seq_t)N;
    for (int i = 1; i <= k; ++i)
    {
        res = res * (n_seq - (seq_t)(i - 1));
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Coefficients of the numerator polynomial in the G.f. */
    static const int coeff[11] = {
        1, 100, 2025, 14400, 44100, 63504,
        44100, 14400, 2025, 100, 1
    };

    seq_t sum = (seq_t)0;
    /* j ranges from 0 to min(10, n) */
    idx_t maxj = n < 10 ? n : 10;
    for (idx_t j = 0; j <= maxj; ++j)
    {
        idx_t N = n - j + 9;             /* argument for binomial */
        seq_t bin = binomial_nk(N, 9);    /* C(N,9) */
        sum += (seq_t)coeff[(int)j] * bin;
    }
    return sum;
}
