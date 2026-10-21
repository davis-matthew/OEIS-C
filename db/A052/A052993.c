/*
 * A052993  a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3), with a(0)=a(1)=1, a(2)=4.
 * https://oeis.org/A052993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3) with a(0)=1, a(1)=1, a(2)=4
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 81, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the initial values */
    if (n == 0 || n == 1) return (seq_t)1;
    if (n == 2)           return (seq_t)4;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t a2 = (seq_t)4;  /* a(2) */

    /* iterate from i = 3 up to n */
    for (idx_t i = 3; i <= n; ++i)
    {
        /* next = a(i) = a2 + 3*a1 - 3*a0 */
        seq_t next = a2 + (seq_t)3 * a1 - (seq_t)3 * a0;

        /* shift the window: a0 <- a1, a1 <- a2, a2 <- next */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
