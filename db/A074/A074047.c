/*
 * A074047  a(n)=a(n-1)*a(n-2)*a(n-3)*(1/a(n-1)+1/a(n-2)+1/a(n-3)) starting
 *          with a(1)=a(2)=a(3)=1.
 * https://oeis.org/A074047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*a(n-2) + a(n-1)*a(n-3) + a(n-2)*a(n-3), with a(1)
 *            = a(2) = a(3) = 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A074047, the OEIS sequence defined by the recurrence above. */
seq_t a(idx_t n)
{
    if (n <= 3)          /* initial terms */
        return (seq_t)1;

    seq_t a1 = 1;        /* a(n-1) at start */
    seq_t a2 = 1;        /* a(n-2) at start */
    seq_t a3 = 1;        /* a(n-3) at start */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(n) = a(n-1)*a(n-2) + a(n-1)*a(n-3) + a(n-2)*a(n-3) */
        next = a1 * a2 + a1 * a3 + a2 * a3;

        /* shift the window: a3 <- a2, a2 <- a1, a1 <- next */
        a3 = a2;
        a2 = a1;
        a1 = next;
    }

    return a1;
}
