/*
 * A145766  Partial sums of A020988.
 * https://oeis.org/A145766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(4^(n+1)-3n-4)/9 Recurrence used: a(0)=0, a(1)=2,
 *            a(2)=12, and for n>2: a(n)=6*a(n-1)-9*a(n-2)+4*a(n-3)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)12;

    /* a0, a1, a2 hold a(n-3), a(n-2), a(n-1) respectively */
    seq_t a0 = (seq_t)0;  /* a(0) */
    seq_t a1 = (seq_t)2;  /* a(1) */
    seq_t a2 = (seq_t)12; /* a(2) */
    seq_t next = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Compute a(i) = 6*a(i-1) - 9*a(i-2) + 4*a(i-3) */
        next = 6 * a2 - 9 * a1 + 4 * a0;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return next;
}
