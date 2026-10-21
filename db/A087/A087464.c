/*
 * A087464  Generalized multiplicative Jacobsthal sequence.
 * https://oeis.org/A087464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*a(n-3) + 8*a(n-6) for n > 5 with initial values a(0)=0,
 *            a(1)=0, a(2)=2, a(3)=0, a(4)=6, a(5)=10
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 66, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* circular buffer for the last six values */
    seq_t buf[6];

    /* initialise the base cases */
    buf[0] = (seq_t)0;   /* a(0) */
    buf[1] = (seq_t)0;   /* a(1) */
    buf[2] = (seq_t)2;   /* a(2) */
    buf[3] = (seq_t)0;   /* a(3) */
    buf[4] = (seq_t)6;   /* a(4) */
    buf[5] = (seq_t)10;  /* a(5) */

    if (n < 6)
        return buf[n];  /* return the initial value directly */

    /* compute a(n) iteratively, keeping only the last six terms */
    for (idx_t i = 6; i <= n; ++i) {
        idx_t idx3 = (i - 3) % 6;  /* index of a(i-3) */
        idx_t idx6 = (i - 6) % 6;  /* index of a(i-6) */
        seq_t val = (seq_t)7 * buf[idx3] + (seq_t)8 * buf[idx6];
        buf[i % 6] = val;          /* store a(i) */
    }

    return buf[n % 6];
}
