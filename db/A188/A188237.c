/*
 * A188237  Number of nondecreasing arrangements of 4 numbers in -(n+2)..(n+2)
 *          with sum zero and not more than two numbers equal.
 * https://oeis.org/A188237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-5) -
 *            a(n-6) Initial values: a(1)=15, a(2)=30, a(3)=52, a(4)=81,
 *            a(5)=121, a(6)=172 The sequence satisfies a linear recurrence of
 *            order 6, so the implementation computes terms iteratively
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases for n = 1..6 */
    static const seq_t base[6] = {15, 30, 52, 81, 121, 172};

    if (n <= 6)
        return base[n - 1];

    /* Circular buffer holding the last six computed terms:
       buf[0] = a(current-5), ..., buf[5] = a(current)
       Initially: a(1) … a(6) */
    seq_t buf[6] = {15, 30, 52, 81, 121, 172};

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute the next term using safe additive order
           to avoid unsigned underflow. */
        seq_t next = 3 * buf[5] - 3 * buf[4];
        next += 2 * buf[3];
        next -= 3 * buf[2];
        next += 3 * buf[1];
        next -= buf[0];

        /* Shift the buffer left and insert the new term at the end */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }
    return buf[5];  /* a(n) */
}
