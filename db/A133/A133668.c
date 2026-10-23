/*
 * A133668  a(n) = a(n-1) - 36*a(n-2), a(0)=1, a(1)=6.
 * https://oeis.org/A133668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 12, i64 n <= 24, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) - 36*a(n-2) for n >= 2,
   with a(0) = 1 and a(1) = 6. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 6;

    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 6;   /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 - (seq_t)36 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
