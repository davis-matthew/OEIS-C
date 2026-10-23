/*
 * A160074  Numerator of Hermite(n, 11/26).
 * https://oeis.org/A160074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 13^n * Hermite(n,11/26) where a(0) = 1, a(1) = 11 and for
 *            n ≥ 2 a(n) = 11 · a(n‑1) – 338·(n‑1) · a(n‑2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)11;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)11;  /* a(1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff = (seq_t)338 * (seq_t)(i - 1);
        a_n = (seq_t)11 * a_nm1 - coeff * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
