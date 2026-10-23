/*
 * A159245  Numerator of Hermite(n, 8/9).
 * https://oeis.org/A159245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 16 a(n+2) = 16*a(n+1) - 162*(n+1)*a(n) for n ≥
 *            0
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 12, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;

    seq_t a0 = (seq_t)1;            /* a(0) */
    seq_t a1 = (seq_t)16;           /* a(1) */
    seq_t an = 0;                   /* placeholder for a(k) */

    for (idx_t k = 2; k <= n; ++k) {
        an = (seq_t)16 * a1 - ((seq_t)162 * (seq_t)(k - 1)) * a0;
        a0 = a1;
        a1 = an;
    }

    return an;
}
