/*
 * A146963  a(n) = ((3 + sqrt(7))^n + (3 - sqrt(7))^n)/2.
 * https://oeis.org/A146963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 2*a(n-2); a(0)=1; a(1)=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)3;
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * (seq_t)6 - a0 * (seq_t)2;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
