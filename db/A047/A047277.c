/*
 * A047277  Numbers that are congruent to {0, 2, 6} mod 7.
 * https://oeis.org/A047277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n>4, with a(1)=0, a(2)=2,
 *            a(3)=6, a(4)=7
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset terms directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)7;

    /* buffer holds a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t buf[4] = { (seq_t)0, (seq_t)2, (seq_t)6, (seq_t)7 };
    seq_t next;

    /* iterate from n=5 up to the requested n */
    for (idx_t i = 5; i <= n; ++i)
    {
        /* a(i) = a(i-1) + a(i-3) - a(i-4) */
        next = buf[3] + buf[1] - buf[0];

        /* shift buffer for the next iteration */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = next;
    }
    return buf[3];   /* a(n) */
}
