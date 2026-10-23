/*
 * A042768  Numerators of continued fraction convergents to sqrt(915).
 * https://oeis.org/A042768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 242*a(n-2) - a(n-4) with a(0)=30, a(1)=121, a(2)=7290,
 *            a(3)=29281
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)30;
    if (n == 1) return (seq_t)121;
    if (n == 2) return (seq_t)7290;
    if (n == 3) return (seq_t)29281;

    seq_t vals[4] = {(seq_t)30, (seq_t)121, (seq_t)7290, (seq_t)29281};

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t newv = (seq_t)242 * vals[(i - 2) & 3] - vals[(i - 4) & 3];
        vals[i & 3] = newv;
    }

    return vals[n & 3];
}
