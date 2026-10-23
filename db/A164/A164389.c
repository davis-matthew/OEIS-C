/*
 * A164389  Number of binary strings of length n with no substrings equal to
 *          0000 or 0101.
 * https://oeis.org/A164389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-3) + a(n-4) for n >= 6 (a(0)=1, a(1)=2,
 *            a(2)=4, a(3)=8, a(4)=14, a(5)=25)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 75, u128 n <= 151,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 0 .. 5 */
    static const seq_t init[6] = {1, 2, 4, 8, 14, 25};

    if (n < 6)
        return init[n];

    /* sliding window of the last 6 values */
    seq_t buf[6];
    for (int i = 0; i < 6; ++i)
        buf[i] = init[i];

    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = a(i-1) + 2*a(i-3) + a(i-4)
           indices: buf[5] = a(i-1), buf[3] = a(i-3), buf[2] = a(i-4) */
        seq_t next = buf[5] + buf[2] + buf[3] + buf[3];

        /* shift left the window */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    return buf[5];
}
