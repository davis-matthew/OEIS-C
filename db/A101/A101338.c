/*
 * A101338  Antidiagonal sums in A101321.
 * https://oeis.org/A101338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 34, u32 n <= 566, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5)
   Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=9, a(4)=20 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)20;

    seq_t prev[5] = { (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)9, (seq_t)20 };
    seq_t next;

    for (idx_t i = 5; i <= n; ++i)
    {
        next = prev[4] * 5 + prev[2] * 10 + prev[0];
        seq_t sub = prev[3] * 10 + prev[1] * 5;
        next -= sub;

        /* shift the window */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = next;
    }

    return next;
}
