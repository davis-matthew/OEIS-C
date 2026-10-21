/*
 * A209648  Number of n X 6 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A209648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) with
 *            a(1)=22, a(2)=484, a(3)=2354, a(4)=7128, a(5)=16830
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 117, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms for n = 1..5 */
    static const seq_t init[5] = { (seq_t)22, (seq_t)484,
                                   (seq_t)2354, (seq_t)7128,
                                   (seq_t)16830 };
    if (n <= 5)
        return init[n - 1];

    /* last[0] = a(k-1), last[1] = a(k-2), ..., last[4] = a(k-5) */
    seq_t last[5];
    last[0] = init[4];     /* a(5)   */
    last[1] = init[3];     /* a(4)   */
    last[2] = init[2];     /* a(3)   */
    last[3] = init[1];     /* a(2)   */
    last[4] = init[0];     /* a(1)   */

    for (idx_t k = 6; k <= n; ++k) {
        seq_t part1 = 5 * last[0];          /* 5 * a(k-1) */
        seq_t part2 = 10 * last[1];         /* 10 * a(k-2) */
        seq_t part3 = 10 * last[2];         /* 10 * a(k-3) */
        seq_t part4 = 5 * last[3];          /* 5 * a(k-4) */

        /* new = 5*a(k-1) - 10*a(k-2) + 10*a(k-3) - 5*a(k-4) + a(k-5) */
        seq_t new_val = part1 - part2 + part3 - part4 + last[4];

        /* shift the window: drop oldest, insert new at front */
        last[4] = last[3];
        last[3] = last[2];
        last[2] = last[1];
        last[1] = last[0];
        last[0] = new_val;
    }
    return last[0];
}
