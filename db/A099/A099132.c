/*
 * A099132  Quintisection of 1/(1-x^5-x^6).
 * https://oeis.org/A099132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) +
 *            a(n-6) with a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=1, a(6)=2
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 72, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first seven terms directly. */
    if (n <= 5) return (seq_t)1;
    if (n == 6) return (seq_t)2;

    /* A sliding window of the last seven computed values. */
    seq_t buf[7];
    buf[0] = (seq_t)1;  /* a(0) */
    buf[1] = (seq_t)1;  /* a(1) */
    buf[2] = (seq_t)1;  /* a(2) */
    buf[3] = (seq_t)1;  /* a(3) */
    buf[4] = (seq_t)1;  /* a(4) */
    buf[5] = (seq_t)1;  /* a(5) */
    buf[6] = (seq_t)2;  /* a(6) */

    seq_t val;
    for (idx_t i = 7; i <= n; ++i) {
        /* Compute the next term from the six most recent ones. */
        val =   5 * buf[6]
             - 10 * buf[5]
             + 10 * buf[4]
             -  5 * buf[3]
             +   buf[2]
             +   buf[1];

        /* Shift the buffer: drop the oldest value (buf[0]) and append val. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = val;
    }

    return buf[6];
}
