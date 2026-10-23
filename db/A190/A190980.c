/*
 * A190980  a(n) = 9*a(n-1) - 3*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 3*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t a0 = (seq_t)0;        /* a(0) */
    seq_t a1 = (seq_t)1;        /* a(1) */
    seq_t ai;
    for (idx_t i = 2; i <= n; ++i) {
        ai = (seq_t)9 * a1 - (seq_t)3 * a0;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
