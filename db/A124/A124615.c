/*
 * A124615  Poincaré series [or Poincare series] P(T_{3,2}; x).
 * https://oeis.org/A124615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in 1/((1-x)^4*(1-x^2)^4*(1-x^3)^2)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t p = 0; 3 * p <= n; ++p) {
        idx_t rem1 = n - 3 * p;
        for (idx_t k = 0; 2 * k <= rem1; ++k) {
            idx_t i = rem1 - 2 * k;
            /* binomial (i+3 choose 3) */
            seq_t bi = ((seq_t)(i + 3) * (i + 2) * (i + 1)) / 6;
            /* binomial (k+3 choose 3) */
            seq_t bk = ((seq_t)(k + 3) * (k + 2) * (k + 1)) / 6;
            seq_t term = bi * bk;
            term *= (seq_t)(p + 1);
            result += term;
        }
    }
    return result;
}
