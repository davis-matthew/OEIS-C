/*
 * A143984  a(0) = 0; thereafter, a(n) = (a(n - 1) - 2)^2 - n.
 * https://oeis.org/A143984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 7, i64 n <= 8, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; a(n) = (a(n‑1) – 2)² – n */
seq_t a(idx_t n)
{
    seq_t cur = 0;                /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t t = cur - (seq_t)2; /* a(n‑1) – 2 */
        cur = t * t - (seq_t)i;  /* (a(n‑1) – 2)² – n */
    }
    return cur;
}
