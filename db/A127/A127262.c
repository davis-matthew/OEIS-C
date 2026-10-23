/*
 * A127262  a(0)=2, a(1)=2, a(n) = 2*a(n-1) + 12*a(n-2).
 * https://oeis.org/A127262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=2, a(1)=2, a(n)=2*a(n-1)+12*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1)
        return (seq_t)2;

    seq_t a0 = (seq_t)2;
    seq_t a1 = (seq_t)2;
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)2 * a1 + (seq_t)12 * a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
