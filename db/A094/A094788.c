/*
 * A094788  Number of (s(0), s(1), ..., s(2n+1)) such that 0 < s(i) < 10 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n+1, s(0) = 1, s(2n+1) = 6.
 * https://oeis.org/A094788
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 20*a(n-3) - 5*a(n-4) with a(2)=1,
 *            a(3)=6, a(4)=27, a(5)=110
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values below the offset are undefined – return 0 */
    if (n < 2) return 0;

    /* initial values */
    if (n == 2) return 1;
    if (n == 3) return 6;
    if (n == 4) return 27;
    if (n == 5) return 110;

    /* sliding window for a(n-4)...a(n-1) */
    seq_t last4[4];
    last4[0] = 1;   /* a(2) */
    last4[1] = 6;   /* a(3) */
    last4[2] = 27;  /* a(4) */
    last4[3] = 110; /* a(5) */

    seq_t next = 0;
    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = 8*a(i-1) - 21*a(i-2) + 20*a(i-3) - 5*a(i-4) */
        next = last4[3];
        next *= 8;
        next -= (last4[2] * 21);
        next += (last4[1] * 20);
        next -= (last4[0] * 5);

        /* shift the window */
        last4[0] = last4[1];
        last4[1] = last4[2];
        last4[2] = last4[3];
        last4[3] = next;
    }
    return next;
}
