/*
 * A052909  Expansion of g.f. (1+x-x^2)/((1-x)*(1-3*x)).
 * https://oeis.org/A052909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(n)=3*a(n-1)+1 for n>=2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the base cases */
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)5;

    /* iterative recurrence for n >= 2 */
    seq_t ans = (seq_t)5;            /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        ans = ans * 3 + 1;           /* a(i) = 3 * a(i-1) + 1 */

    return ans;
}
