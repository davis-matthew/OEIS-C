/*
 * A221357  Number of n X 2 arrays of occupancy after each element moves to
 *          some horizontal, vertical or antidiagonal neighbor, with no
 *          2-loops and with no occupancy greater than 2.
 * https://oeis.org/A221357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 38*a(n-2) + 14*a(n-3) + 59*a(n-4) - 21*a(n-5)
 *            - 33*a(n-6) + 16*a(n-7) Initial values: a(1)=0, a(2)=5, a(3)=39,
 *            a(4)=292, a(5)=2088, a(6)=14799, a(7)=104532
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for undefined indices just return 0 */
    if (n < 1U)
        return 0;

    /* first 7 terms can be returned directly */
    static const seq_t init[8] = {0, 0, 5, 39, 292, 2088, 14799, 104532};
    if (n <= 7U)
        return init[n];

    /* sliding window for the last 7 computed values:
       w[6] = a(i-1), w[5] = a(i-2), ..., w[0] = a(i-7)  */
    seq_t w[7];
    w[0] = init[1];
    w[1] = init[2];
    w[2] = init[3];
    w[3] = init[4];
    w[4] = init[5];
    w[5] = init[6];
    w[6] = init[7];

    seq_t val = 0;
    for (idx_t i = 8; i <= n; ++i)
    {
        val = 12 * w[6] - 38 * w[5] + 14 * w[4] + 59 * w[3]
              - 21 * w[2] - 33 * w[1] + 16 * w[0];

        /* shift the window for next iteration */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = w[5];
        w[5] = w[6];
        w[6] = val;
    }

    return val;
}
