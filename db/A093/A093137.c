/*
 * A093137  Expansion of (1-7*x)/((1-x)*(1-10*x)).
 * https://oeis.org/A093137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n + 2)/3 Recurrence: a(0) = 1, a(n) = 10*a(n-1) - 6
 *            for n ≥ 1 (Both are equivalent to the generating function
 *            (1-7x)/((1-x)(1-10x)).)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* a(0) = 1 */
        return (seq_t)1;

    seq_t val = (seq_t)1;   /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* a(i) = 10 * a(i-1) - 6 */
        val = val * (seq_t)10 - (seq_t)6;
    }
    return val;
}
