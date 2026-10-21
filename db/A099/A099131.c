/*
 * A099131  Quintisection and binomial transform of 1/(1-x^4-x^5).
 * https://oeis.org/A099131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 4*a(n-4) + a(n-5)
 *            initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial terms directly */
    static const seq_t init[5] = {1, 1, 1, 1, 2};

    if (n < 5)
        return init[n];

    /* Rolling buffer for the last five terms */
    seq_t buf[5];
    buf[0] = init[0]; /* a(n-5) */
    buf[1] = init[1]; /* a(n-4) */
    buf[2] = init[2]; /* a(n-3) */
    buf[3] = init[3]; /* a(n-2) */
    buf[4] = init[4]; /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 5 * buf[4] - 10 * buf[3] + 10 * buf[2] - 4 * buf[1] + buf[0];

        /* Shift the buffer: drop the oldest term */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }

    return buf[4]; /* a(n) */
}
