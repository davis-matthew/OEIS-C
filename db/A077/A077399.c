/*
 * A077399  Triangular numbers that are 1/7 of triangular numbers.
 * https://oeis.org/A077399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 254*a(n-2) - a(n-4) + 18; Initial values: a(0)=0, a(1)=3,
 *            a(2)=15, a(3)=780
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)780;

    /* Ring buffer holding a[i-4], a[i-3], a[i-2], a[i-1] */
    seq_t buf[4];
    buf[0] = (seq_t)0;   /* a(0)   */
    buf[1] = (seq_t)3;   /* a(1)   */
    buf[2] = (seq_t)15;  /* a(2)   */
    buf[3] = (seq_t)780; /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 254 * a(i-2) - a(i-4) + 18 */
        seq_t new_val = (seq_t)254 * buf[2];
        new_val = new_val - buf[0];
        new_val = new_val + (seq_t)18;

        /* Shift the ring buffer to the left and store the new term. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = new_val;
    }

    return buf[3]; /* a(n) is now in buf[3] */
}
