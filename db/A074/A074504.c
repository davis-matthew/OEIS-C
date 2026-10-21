/*
 * A074504  a(n) = 1^n + 2^n + 8^n.
 * https://oeis.org/A074504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 3, a(1) = 11, a(n) = 10*a(n-1) - 16*a(n-2) + 7 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;
    if (n == 1)
        return (seq_t)11;

    seq_t prev2 = (seq_t)3;   /* a(0) */
    seq_t prev1 = (seq_t)11;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)10 - prev2 * (seq_t)16 + (seq_t)7;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
