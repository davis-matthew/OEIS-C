/*
 * A182462  a(n) = 3*a(n-1) - 2*a(n-2) with a(0)=20 and a(1)=50.
 * https://oeis.org/A182462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2); a(0) = 20, a(1) = 50
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)20;
    if (n == 1) return (seq_t)50;

    seq_t a0 = (seq_t)20;
    seq_t a1 = (seq_t)50;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t an = a1 * (seq_t)3 - a0 * (seq_t)2;
        a0 = a1;
        a1 = an;
    }

    return a1;
}
