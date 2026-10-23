/*
 * A160865  128*P_11(n), 128 times the Legendre polynomial of order 11 at n.
 * https://oeis.org/A160865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0}^{min(10,n-1)} P_j * C(n-j+10,11) where C(k,11) is
 *            the binomial coefficient (k choose 11) and the numbers P_j form
 *            the numerator polynomial P(x) = 128 + 43792327x + 5240462124x² +
 *            90099630276x³ + 429789855348x⁴ + 709564273194x⁵ + 429789855348x⁶
 *            + 90099630276x⁷ + 5240462124x⁸ + 43792...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 21, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1)=128, a(2)=43793863, … as given in OEIS. */

#include <stddef.h>

static const seq_t P[11] = {
    128ULL,
    43792327ULL,
    5240462124ULL,
    90099630276ULL,
    429789855348ULL,
    709564273194ULL,
    429789855348ULL,
    90099630276ULL,
    5240462124ULL,
    43792327ULL,
    128ULL
};

seq_t a(idx_t n)
{
    /* For n = 0 the sum is empty → 0. */
    if (n == 0) return 0;

    /* j ranges from 0 to min(10, n-1). */
    idx_t max_j = (n > 10 ? 10 : n - 1);
    seq_t result = 0;

    for (idx_t j = 0; j <= max_j; ++j) {
        /* Compute the binomial coefficient C(n-j+10, 11). */
        idx_t m = n - j + 10;          /* m >= 11 for j <= n-1 */
        seq_t binom = 1;
        for (idx_t i = 1; i <= 11; ++i) {
            binom = binom * (seq_t)(m - 11 + i);
            binom = binom / (seq_t)i; /* exact division */
        }
        result += P[j] * binom;
    }

    return result;
}
