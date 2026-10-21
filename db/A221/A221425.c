/*
 * A221425  Number of n X 2 arrays of occupancy after each element moves to
 *          some horizontal or vertical neighbor, without consecutive moves in
 *          the same direction.
 * https://oeis.org/A221425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 6*a(n-2) - 5*a(n-4) + a(n-5); Initial values:
 *            a(1)=1, a(2)=9, a(3)=42, a(4)=217, a(5)=1116
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset 1 explicit values */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)42;
    if (n == 4) return (seq_t)217;
    if (n == 5) return (seq_t)1116;

    /* Sliding window: w[0]=a(n-5), w[1]=a(n-4), w[2]=a(n-3),
       w[3]=a(n-2), w[4]=a(n-1) */
    seq_t w[5];
    w[0] = (seq_t)1;    /* a(1)   */
    w[1] = (seq_t)9;    /* a(2)   */
    w[2] = (seq_t)42;   /* a(3)   */
    w[3] = (seq_t)217;  /* a(4)   */
    w[4] = (seq_t)1116; /* a(5)   */

    seq_t next;
    idx_t i;

    for (i = 6; i <= n; ++i) {
        next = 4 * w[4] + 6 * w[3] - 5 * w[1] + w[0];
        /* shift left */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = next;
    }
    return w[4];
}
