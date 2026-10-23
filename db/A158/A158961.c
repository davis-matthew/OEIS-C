/*
 * A158961  Numerator of Hermite(n, 2/5).
 * https://oeis.org/A158961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 4; for n ≥ 2, a(n) = 4*a(n‑1) – 50*(n‑1)*a(n‑2)
 *            (Hermite(n,2/5) * 5^n)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 28,
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
        return (seq_t)4;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)4;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term = 50 * (seq_t)(i - 1) * prev2;
        seq_t next = 4 * prev1 - term;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
