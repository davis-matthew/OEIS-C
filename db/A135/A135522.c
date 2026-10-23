/*
 * A135522  a(n) = 2*a(n-1) + 3*a(n-2), with a(0) = 2 and a(1) = 3.
 * https://oeis.org/A135522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2), a(0) = 2, a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial terms explicitly. */
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)3;

    /* Iterate the recurrence until reaching the desired index. */
    seq_t a0 = (seq_t)2;        /* a(0) */
    seq_t a1 = (seq_t)3;        /* a(1) */
    seq_t an;                   /* will hold a(k) as we go along */

    for (idx_t k = 2; k <= n; ++k)
    {
        an = (seq_t)2 * a1 + (seq_t)3 * a0;  /* a(k) = 2*a(k-1) + 3*a(k-2) */
        a0 = a1;
        a1 = an;
    }

    return an;   /* a(n) */
}
