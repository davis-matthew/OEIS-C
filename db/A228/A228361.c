/*
 * A228361  The number of all possible covers of L-length line segment by
 *          2-length line segments with allowed gaps < 2.
 * https://oeis.org/A228361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) for n >= 5, a(0)=0, a(1)=0, a(2)=1,
 *            a(3)=2, a(4)=2
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3 || n == 4) return 2;

    seq_t buf[5];
    buf[0] = 0;   /* a0 */
    buf[1] = 0;   /* a1 */
    buf[2] = 1;   /* a2 */
    buf[3] = 2;   /* a3 */
    buf[4] = 2;   /* a4 */

    for (idx_t i = 5; i <= n; ++i) {
        buf[i % 5] = buf[(i - 2) % 5] + buf[(i - 3) % 5];
    }

    return buf[n % 5];
}
