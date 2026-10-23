/*
 * A100701  a(n) = a(n-1) + a(n-2) + a(n-1)*a(n-2) for n>=2; a(0)=2, a(1)=3.
 * https://oeis.org/A100701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-1)*a(n-2), a(0)=2, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;

    seq_t a0 = (seq_t)2;
    seq_t a1 = (seq_t)3;
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = a0 + a1 + a0 * a1;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
