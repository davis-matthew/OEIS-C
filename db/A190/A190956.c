/*
 * A190956  a(n) = 10*a(n-1) + 6*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 6*a(n-2), with a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
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
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i) {
        ai = (seq_t)10 * a1 + (seq_t)6 * a0;
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
