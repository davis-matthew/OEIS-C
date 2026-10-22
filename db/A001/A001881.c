/*
 * A001881  Coefficients of Bessel polynomials y_n (x).
 * https://oeis.org/A001881
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n-3,2)*(2*n-5)!!/5!!, i.e. a(n) =
 *            binomial(n-3,2) * 7*9*11*...*(2n-5)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 5) return 0;

    idx_t p = n - 3, q = n - 4;
    seq_t result;
    if (p % 2 == 0)
        result = (seq_t)(p / 2) * (seq_t)q;
    else
        result = (seq_t)p * (seq_t)(q / 2);

    /* (2n-5)!!/15 = 7*9*...*(2n-5) (empty product for n <= 5) */
    for (idx_t k = 7; k + 5 <= 2 * n; k += 2) {
        result *= (seq_t)k;
    }

    return result;
}
