/*
 * A008531  Coordination sequence for {A_4}* lattice.
 * https://oeis.org/A008531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^4 c[i] * binom(n-i+3,3) with coefficients c =
 *            {1,6,16,6,1} (derived from (1+6x+16x²+6x³+x⁴)/(1-x)⁴)
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 950, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const int coeff[5] = {1, 6, 16, 6, 1};

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 0; i <= 4 && i <= n; ++i)
    {
        idx_t delta = n - i;          /* n-i >= 0 */
        seq_t k = (seq_t)(delta + 3);  /* k >= 3 for all valid n-i */
        seq_t bin = k * (k - 1) * (k - 2) / 6; /* binom(k,3) */
        result += (seq_t)coeff[i] * bin;
    }
    return result;
}
