/*
 * A016377  301st cyclotomic polynomial.
 * https://oeis.org/A016377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(0)=1; for 1 ≤ n ≤ 252, a(n) = (n%43==0?1:0) –
 *            sum_{i=1}^{min(6,n)} a(n‑i); For n > 252, a(n)=0. These are the
 *            coefficients of the 301st cyclotomic polynomial Φ_301(x)
 * Range:     i8 n <= 252, i16 n <= 252, i32 n <= 252, i64 n <= 252,
 *            i128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Static storage for the coefficients of Φ_301(x).  The polynomial has
       degree φ(301) = (7–1)(43–1) = 252, so we need 253 coefficients. */
    static seq_t coeff[253];
    static int ready = 0;

    if (!ready) {
        const idx_t max_deg = 252;
        for (idx_t k = 0; k <= max_deg; ++k) {
            seq_t sum = 0;
            for (idx_t i = 1; i <= 6 && k >= i; ++i) {
                sum += coeff[k - i];
            }
            /* c(k) = 1 if k is a multiple of 43 and k ≤ 258 (i.e. k/43 ≤ 6),
               otherwise 0.  Only the first 7 multiples affect coefficients
               up to degree 252. */
            seq_t c = ((k % 43 == 0) && (k / 43 <= 6)) ? (seq_t)1 : 0;
            coeff[k] = c - sum;
        }
        ready = 1;
    }

    if (n <= 252) {
        return coeff[(idx_t)n];
    }
    return (seq_t)0;
}
