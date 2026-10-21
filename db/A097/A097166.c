/*
 * A097166  Expansion of g.f. (1+2*x)/((1-x)*(1-10*x)).
 * https://oeis.org/A097166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*10^n - 1)/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n) = 10*a(n-1) + 3 for n > 0 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t value = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        value = value * (seq_t)10 + (seq_t)3;
    }
    return value;
}
