/*
 * A116705  Number of permutations of length n which avoid the patterns 1234,
 *          4312.
 * https://oeis.org/A116705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (21*n^8 - 48*n^7 - 966*n^6 + 7728*n^5 - 8211*n^4 -
 *            72912*n^3 + 311556*n^2 - 418608*n + 241920)/60480
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom8(seq_t N)
{
    /* Computes binomial coefficient C(N,8) using 128-bit intermediates. */
#if defined(__SIZEOF_INT128__) || defined(__INT128__)
    unsigned __int128 prod = 1;
#else
    unsigned long long prod = 1;
#endif
    for (int i = 1; i <= 8; ++i) {
#if defined(__SIZEOF_INT128__) || defined(__INT128__)
        prod *= (unsigned __int128)(N - (seq_t)(i - 1));
        prod /= (unsigned __int128)i;
#else
        prod *= (unsigned long long)(N - (seq_t)(i - 1));
        prod /= (unsigned long long)i;
#endif
    }
    return (seq_t)prod;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* behavior below offset is irrelevant */

    /* Coefficients of the polynomial Q(x) = 4x^8 - 19x^7 + 32x^6 - 39x^5
       + 62x^4 - 44x^3 + 24x^2 - 7x + 1. */
    static const long long coeff[9] = {
        1LL, -7LL, 24LL, -44LL, 62LL, -39LL, 32LL, -19LL, 4LL
    };

    signed __int128 total = 0;
    for (int j = 0; j < 9; ++j) {
        if (n >= (idx_t)(j + 1)) {               /* ensures N >= 8 */
            seq_t N = n - (idx_t)j + 7;           /* N = n - j + 7 */
            seq_t bin = binom8(N);                /* C(N,8) */
            total += (signed __int128)coeff[j] * (signed __int128)bin;
        }
    }
    return (seq_t)total;
}
