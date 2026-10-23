/*
 * A026042  a(n) = (d(n)-r(n))/5, where d = A026040 and r is the periodic
 *          sequence with fundamental period (4,0,4,3,4).
 * https://oeis.org/A026042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-5) - 3*a(n-6) +
 *            3*a(n-7) - a(n-8). Initial values: a(1)=4, a(2)=8, a(3)=12,
 *            a(4)=19, a(5)=28, a(6)=40, a(7)=56, a(8)=74. The recurrence
 *            comes from the rational generating function listed in the OEIS
 *            entry
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first eight terms of the sequence */
    static const seq_t init[8] = {4, 8, 12, 19, 28, 40, 56, 74};

    /* for n <= 8 just return the predefined value */
    if (n <= 8)
        return init[n - 1];

    /* ring buffer of the last 8 values */
    seq_t buf[8];
    for (int i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 9; i <= n; ++i)
    {
        /* compute next value using the recurrence */
        seq_t next = 3 * buf[7]      /* a(n-1) */
                   - 3 * buf[6]      /* a(n-2) */
                   + buf[5]          /* a(n-3) */
                   + buf[3]          /* a(n-5) */
                   - 3 * buf[2]      /* a(n-6) */
                   + 3 * buf[1]      /* a(n-7) */
                   - buf[0];         /* a(n-8) */

        /* shift buffer: discard oldest, make room for new */
        for (int j = 0; j < 7; ++j)
            buf[j] = buf[j + 1];
        buf[7] = next;
    }

    return buf[7];
}
