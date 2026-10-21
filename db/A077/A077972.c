/*
 * A077972  Expansion of 1/(1+x-x^2+2*x^3).
 * https://oeis.org/A077972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1 a(1) = -1 a(2) = 2 a(n) = –a(n‑1) + a(n‑2) – 2·a(n‑3)
 *            for n ≥ 3 (recurrence derived from (1 + x – x² + 2x³) · Σ a(n)
 *            xⁿ = 1)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)2;

    seq_t a0 = (seq_t)1;   /* a(n‑3)  */
    seq_t a1 = (seq_t)-1;  /* a(n‑2)  */
    seq_t a2 = (seq_t)2;   /* a(n‑1)  */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t w = -a2 + a1 - ((seq_t)2 * a0);  /* a(n) = –a(n‑1) + a(n‑2) – 2·a(n‑3) */
        a0 = a1;
        a1 = a2;
        a2 = w;
    }
    return a2;
}
