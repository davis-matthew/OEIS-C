/*
 * A126899  Coordination sequence for 6-dimensional cyclotomic lattice
 *          Z[zeta_9].
 * https://oeis.org/A126899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{6} c[i] * binomial(n-i+5,5) with c =
 *            {1,3,6,7,6,3,1}. binomial(m,5) is computed multiplicatively to
 *            avoid overflow
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* coefficients of (x²+x+1)³ */
    static const seq_t c[7] = {1, 3, 6, 7, 6, 3, 1};

    seq_t result = 0;

    for (idx_t i = 0; i < 7; ++i) {
        if (n < i) break;                     /* term does not contribute */
        seq_t m = (seq_t)(n - i + 5);          /* m = n-i+5 >= 5 */
        if (m < 5) continue;                  /* binomial is zero for m<5 */

        /* binomial(m,5) via product/quotient to keep intermediates small */
        seq_t bin = 1;
        for (idx_t k = 0; k < 5; ++k) {
            bin = bin * (m - k) / (k + 1);
        }

        result += c[i] * bin;
    }

    return result;
}
