/*
 * A215878  Lengths of loops in the P2 Penrose tiling.
 * https://oeis.org/A215878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   a(n+4) = 5*a(n+2) + 6*a(n), with a(1)=10, a(2)=20, a(3)=80,
 *            a(4)=100
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    static const seq_t first[5] = {0, 10, 20, 80, 100};
    if (n <= 4) {
        return first[n];
    }

    /* Use a circular buffer of the last four values */
    seq_t buf[4] = {10, 20, 80, 100};

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = ((seq_t)5) * buf[2] + ((seq_t)6) * buf[0];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = next;
    }
    return buf[3];
}
