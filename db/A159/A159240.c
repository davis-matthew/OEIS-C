/*
 * A159240  Numerator of Hermite(n, 5/9).
 * https://oeis.org/A159240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1 a(1) = 10 a(n) = 10 * a(n‑1) – 162 * (n‑1) * a(n‑2) for
 *            n >= 2 (This recurrence follows from the EGF exp(10x–81x²).)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 10;

    seq_t a_prev = 1;   /* a(0) */
    seq_t a_curr = 10;  /* a(1) */

    for (idx_t i = 1; i < n; ++i) {  /* i corresponds to n‑1 in the recurrence */
        seq_t factor = (seq_t)162 * (seq_t)i;   /* 162*(n‑1) */
        seq_t next = (seq_t)10 * a_curr - factor * a_prev;
        a_prev = a_curr;
        a_curr = next;
    }
    return a_curr;
}
