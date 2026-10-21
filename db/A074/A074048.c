/*
 * A074048  Pentanacci numbers with initial conditions a(0)=5, a(1)=1, a(2)=3,
 *          a(3)=7, a(4)=15.
 * https://oeis.org/A074048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5); initial values:
 *            a(0)=5, a(1)=1, a(2)=3, a(3)=7, a(4)=15
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 65, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[5] = {5, 1, 3, 7, 15};

    if (n < 5) {
        return init[n];
    }

    seq_t buf[5];
    buf[0] = init[0];
    buf[1] = init[1];
    buf[2] = init[2];
    buf[3] = init[3];
    buf[4] = init[4];

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = buf[0] + buf[1] + buf[2] + buf[3] + buf[4];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }

    return buf[4];
}
