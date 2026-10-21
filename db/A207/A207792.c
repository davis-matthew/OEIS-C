/*
 * A207792  Number of (n+1) X 2 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having an unequal number of clockwise and counterclockwise edge
 *          increases.
 * https://oeis.org/A207792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 61*a(n-2) + 83*a(n-3) + 272*a(n-4) + 336*a(n-5)
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=256, a(2)=2208, a(3)=20800, a(4)=184416, a(5)=1714176 */

seq_t a(idx_t n)
{
    /* Return the first five terms directly. */
    if (n == 1) return (seq_t)256;
    if (n == 2) return (seq_t)2208;
    if (n == 3) return (seq_t)20800;
    if (n == 4) return (seq_t)184416;
    if (n == 5) return (seq_t)1714176;

    /* Circular buffer holding the last five computed values. */
    seq_t v[5] = { (seq_t)256, (seq_t)2208, (seq_t)20800,
                   (seq_t)184416, (seq_t)1714176 };

    seq_t next;
    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = a(i-1) + 61*a(i-2) + 83*a(i-3) + 272*a(i-4) + 336*a(i-5) */
        next = v[4]
             + (seq_t)61 * v[3]
             + (seq_t)83 * v[2]
             + (seq_t)272 * v[1]
             + (seq_t)336 * v[0];

        /* Shift the window circularly. */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = next;
    }
    return v[4];
}
