/*
 * A219940  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or diagonal neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..3 2 X n array.
 * https://oeis.org/A219940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) (n>8) Initial values: a(1)=10, a(2)=23, a(3)=64,
 *            a(4)=196, a(5)=459, a(6)=918, a(7)=1660, a(8)=2802
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 147, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    static const seq_t init[8] = {10, 23, 64, 196, 459, 918, 1660, 2802};
    if (n <= 8)
        return init[n - 1];

    /* sliding window of the last six values:
       when n == 9, buf[0] = a(8), buf[1] = a(7), ..., buf[5] = a(3) */
    seq_t buf[6];
    buf[0] = init[7]; /* a8 */
    buf[1] = init[6]; /* a7 */
    buf[2] = init[5]; /* a6 */
    buf[3] = init[4]; /* a5 */
    buf[4] = init[3]; /* a4 */
    buf[5] = init[2]; /* a3 */

    for (idx_t i = 9; i <= n; ++i) {
        /* compute a(i) using the recurrence with only unsigned arithmetic
           (the positive sum always exceeds the negative sum for all n≥9) */
        seq_t pos = (seq_t)6 * buf[0] + (seq_t)20 * buf[2] + (seq_t)6 * buf[4];
        seq_t neg = (seq_t)15 * buf[1] + (seq_t)15 * buf[3] + buf[5];
        seq_t new_val = pos - neg;

        /* shift the window */
        buf[5] = buf[4];
        buf[4] = buf[3];
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = new_val;
    }
    return buf[0];
}
