/*
 * A025594  Number of n-move bishop paths on 8x8 board from given corner to
 *          opposite corner.
 * https://oeis.org/A025594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * * a(0) = 0, a(1) = 1,
 *            a(2) = 6, a(3) = 43, a(4) = 324, * a(5) = 2565, a(6) = 21202,
 *            a(7) = 181071 * * For n ≥ 8: * a(n) = 21·a(n‑1) – 144·a(n‑2) +
 *            302·a(n‑3) + 355·a(n‑4) * – 1179·a(n‑5) – 882·a(n‑6) +
 *            376·a(n‑7) + 192·a(n‑8)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms of the sequence */
    static const seq_t init[8] = {0,1,6,43,324,2565,21202,181071};

    if (n < 8)
        return init[(idx_t)n];

    /* buffer holds the eight most recent terms:
     * buf[0] = a(i-1), buf[1] = a(i-2), …, buf[7] = a(i-8)
     */
    seq_t buf[8];
    buf[0] = init[7]; /* a(7) */
    buf[1] = init[6]; /* a(6) */
    buf[2] = init[5]; /* a(5) */
    buf[3] = init[4]; /* a(4) */
    buf[4] = init[3]; /* a(3) */
    buf[5] = init[2]; /* a(2) */
    buf[6] = init[1]; /* a(1) */
    buf[7] = init[0]; /* a(0) */

    /* iterate from n = 8 up to the requested n */
    for (idx_t i = 8; i <= n; ++i)
    {
        /* compute the two partial sums using only additions and
           subtractions of unsigned values. It is guaranteed that
           the positive sum is never smaller than the negative sum,
           so the final subtraction does not underflow. */
        seq_t pos_sum = buf[0]*(seq_t)21
                       + buf[2]*(seq_t)302
                       + buf[3]*(seq_t)355
                       + buf[6]*(seq_t)376
                       + buf[7]*(seq_t)192;

        seq_t neg_sum = buf[1]*(seq_t)144
                       + buf[4]*(seq_t)1179
                       + buf[5]*(seq_t)882;

        seq_t next = pos_sum - neg_sum;  /* a(i) */

        /* shift the buffer for the next iteration */
        for (int j = 7; j >= 1; --j)
            buf[j] = buf[j-1];
        buf[0] = next;
    }

    return buf[0];   /* a(n) */
}
