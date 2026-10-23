/*
 * A199403  Binary XOR of (2^k - (-1)^k)/3 as k varies from 1 to n.
 * https://oeis.org/A199403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-4) - 2*a(n-5) * with initial values: *
 *            a(1) = 1 * a(2) = 0 * a(3) = 3 * a(4) = 6 * a(5) = 13
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used – offset starts at 1 */

    /*  Handle the first five terms directly  */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)6;
    if (n == 5) return (seq_t)13;

    /*  Buffer holding a(i-1) … a(i-5)  */
    seq_t buf[5];
    buf[0] = (seq_t)13;   /* a(5)  */
    buf[1] = (seq_t)6;    /* a(4)  */
    buf[2] = (seq_t)3;    /* a(3)  */
    buf[3] = (seq_t)0;    /* a(2)  */
    buf[4] = (seq_t)1;    /* a(1)  */

    idx_t i;
    for (i = 6; i <= n; ++i) {
        /*  Compute a(i) = 2*a(i-1) + a(i-4) - 2*a(i-5)  */
        seq_t sum   = buf[0] * (seq_t)2 + buf[3];
        seq_t next  = sum - buf[4] * (seq_t)2;

        /*  Shift the window for the next iteration  */
        buf[4] = buf[3];
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = next;
    }

    return buf[0];   /*  a(n)  */
}
