/*
 * A159582  Expansion of (1+6*x+x^2-2*x^3)/((x^2+2*x-1)*(x^2-2*x-1)),
 *          bisection is NSW numbers.
 * https://oeis.org/A159582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-2) - a(n-4) for n > 3, with initial values a(0)=1,
 *            a(1)=6, a(2)=7, a(3)=34
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)34;

    seq_t buf[4] = { (seq_t)1, (seq_t)6, (seq_t)7, (seq_t)34 };
    for (idx_t i = 4; i <= n; ++i)
    {
        buf[i & 3] = ((seq_t)6 * buf[(i - 2) & 3]) - buf[(i - 4) & 3];
    }
    return buf[n & 3];
}
