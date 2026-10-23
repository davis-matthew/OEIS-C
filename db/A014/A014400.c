/*
 * A014400  Inverse of 391st cyclotomic polynomial.
 * https://oeis.org/A014400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   Let s(m) = floor(m/391)+1 if m ≥ 0 else 0. Then a(n) = s(n) –
 *            s(n‑17) – s(n‑23) + s(n‑40). This yields the periodic sequence
 *            of inverse 391st cyclotomic polynomial coefficients
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)(n / 391) + 1;       /* s(n)   */
    if (n >= 17) {
        idx_t m = n - 17;
        result -= ((seq_t)(m / 391) + 1);     /* – s(n‑17)  */
    }
    if (n >= 23) {
        idx_t m = n - 23;
        result -= ((seq_t)(m / 391) + 1);     /* – s(n‑23)  */
    }
    if (n >= 40) {
        idx_t m = n - 40;
        result += ((seq_t)(m / 391) + 1);     /* + s(n‑40)  */
    }
    return result;
}
