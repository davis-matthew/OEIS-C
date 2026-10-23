/*
 * A182464  a(n) = 3*a(n-1) - 2*a(n-2) with a(0)=24 and a(1)=60.
 * https://oeis.org/A182464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2), a(0)=24, a(1)=60
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)24;
    if (n == 1)
        return (seq_t)60;

    seq_t a0 = (seq_t)24;
    seq_t a1 = (seq_t)60;
    seq_t aN;

    for (idx_t i = 2; i <= n; ++i) {
        aN = (seq_t)3 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = aN;
    }
    return a1;
}
