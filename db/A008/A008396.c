/*
 * A008396  Crystal ball sequence for A_10 lattice.
 * https://oeis.org/A008396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0}^{min(n,10)} c[j] * binom(n-j+10,10) with
 *            coefficients: c[0]=1, c[1]=100, c[2]=2025, c[3]=14400,
 *            c[4]=44100, c[5]=63504, c[6]=44100, c[7]=14400, c[8]=2025,
 *            c[9]=100, c[10]=1. This comes from the generating function (1 +
 *            100x + 2025x² + 14400x³ + 44100x⁴ + 63504x⁵ + 44100x⁶ +
 *            14400x⁷...
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 11, u64 n <= 113, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* computes binomial(N, 10) for N >= 10 */
static seq_t binom10(idx_t N)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= 10; ++i) {
        res = res * (seq_t)(N - i + 1) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* coefficients of the numerator polynomial */
    static const seq_t coeffs[11] = {
        1ULL,   100ULL, 2025ULL, 14400ULL, 44100ULL,
        63504ULL, 44100ULL, 14400ULL, 2025ULL, 100ULL, 1ULL
    };

    seq_t sum = 0;
    idx_t max_j = (n < 10 ? n : 10);

    for (idx_t j = 0; j <= max_j; ++j) {
        idx_t N = n - j + 10;
        seq_t bin = binom10(N);
        sum += coeffs[j] * bin;
    }

    return sum;
}
