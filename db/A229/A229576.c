/*
 * A229576  Number of defective 4-colorings of an n X 6 0..3 array connected
 *          horizontally, vertically, diagonally and antidiagonally with
 *          exactly one mistake, and colors introduced in row-major 0..3
 *          order.
 * https://oeis.org/A229576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 12*a(n-3) - 4*a(n-4) for n>7
 *            Initial values: a(1) = 70 a(2) = 544 a(3) = 757 a(4) = 1064 a(5)
 *            = 1576 a(6) = 2440 a(7) = 4048
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return early for the first seven terms */
    if (n == 1) return 70;
    if (n == 2) return 544;
    if (n == 3) return 757;
    if (n == 4) return 1064;
    if (n == 5) return 1576;
    if (n == 6) return 2440;
    if (n == 7) return 4048;

    /* Use a sliding window of the last four values */
    seq_t a4 = 1064;   /* a(n-4)   */
    seq_t a5 = 1576;   /* a(n-3)   */
    seq_t a6 = 2440;   /* a(n-2)   */
    seq_t a7 = 4048;   /* a(n-1)   */

    for (idx_t i = 8; i <= n; ++i)
    {
        /* Compute a(i) = 6*a(i-1) - 13*a(i-2) + 12*a(i-3) - 4*a(i-4) */
        seq_t t1 = 6 * a7 + 12 * a5;      /* 6*a(i-1) + 12*a(i-3)   */
        seq_t t2 = 13 * a6 + 4 * a4;      /* 13*a(i-2) + 4*a(i-4)   */
        seq_t new_val = t1 - t2;          /* a(i)  */

        /* Shift the window */
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = new_val;
    }

    return a7;  /* a(n) */
}
