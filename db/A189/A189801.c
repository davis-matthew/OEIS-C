/*
 * A189801  a(n) = 6*a(n-1) + 9*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A189801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 9*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t an;
    const seq_t six = (seq_t)6;
    const seq_t nine = (seq_t)9;

    for (idx_t i = 2; i <= n; ++i) {
        an = six * a1 + nine * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
