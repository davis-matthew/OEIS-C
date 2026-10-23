/*
 * A015412  Inverse of 1403rd cyclotomic polynomial.
 * https://oeis.org/A015412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (⌊n/1403⌋ + 1) - (⌊(n-23)/1403⌋ + 1) if n ≥ 23 -
 *            (⌊(n-61)/1403⌋ + 1) if n ≥ 61 + (⌊(n-84)/1403⌋ + 1) if n ≥ 84
 *            This follows from 1/Φ_1403(x) = (1‑x⁻¹)(1‑x²³)(1‑x⁶¹)/(1‑x¹⁴⁰³)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* term with 1/(1‑x) */
    seq_t res = (seq_t)(n / 1403 + 1);

    if (n >= 23)
        res -= (seq_t)((n - 23) / 1403 + 1);

    if (n >= 61)
        res -= (seq_t)((n - 61) / 1403 + 1);

    if (n >= 84)
        res += (seq_t)((n - 84) / 1403 + 1);

    return res;
}
