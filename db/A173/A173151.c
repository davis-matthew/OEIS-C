/*
 * A173151  a(n) = a(n-1) - [a(n-1)/2] + a(n-2) - [a(n-5)/2] where [k] =
 *          floor(k).
 * https://oeis.org/A173151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1020, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) - a(n-5) - a(n-6) + a(n-7)
 * Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=2, a(4)=3, a(5)=4, a(6)=5
 */
seq_t a(idx_t n)
{
    /* The first seven terms */
    static const seq_t init[7] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2,
                                   (seq_t)3, (seq_t)4, (seq_t)5 };

    /* Return a small precomputed value directly. */
    if (n < 7)
        return init[n];

    /* Keep a sliding window of the last seven values.  The buffer
     * index 0 holds a(n-7) and index 6 holds a(n-1) for the current
     * n.  All operations are performed with positive intermediate
     * values so that unsigned arithmetic does not underflow. */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) using the recurrence:
         * a(i) = a(i-1)+a(i-2)-a(i-3)+a(i-4)-a(i-5)-a(i-6)+a(i-7)   */
        seq_t sum_pos = buf[6] + buf[5] + buf[3] + buf[0];   /* +a(i-1)+a(i-2)+a(i-4)+a(i-7) */
        seq_t sum_neg = buf[4] + buf[2] + buf[1];            /* +a(i-3)+a(i-5)+a(i-6) */
        seq_t next = sum_pos - sum_neg;                      /* set equals recurrence */

        /* Shift the buffer left by one position and insert the newly
         * computed term at the end.  After the shift, buf[0] becomes
         * a(i-6) and buf[6] becomes a(i). */
        for (int k = 0; k < 6; ++k)
            buf[k] = buf[k + 1];
        buf[6] = next;
    }

    return buf[6];
}
