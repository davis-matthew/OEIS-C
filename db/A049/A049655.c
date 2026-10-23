/*
 * A049655  a(n) = (F(8n+2)-1)/3, where F=A000045 (the Fibonacci sequence).
 * https://oeis.org/A049655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=18, a(n)=47*a(n-1)-a(n-2)+15 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)18;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)18;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)47 * prev1 - prev2 + (seq_t)15;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
