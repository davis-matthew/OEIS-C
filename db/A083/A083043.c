/*
 * A083043  Integers y such that 11*x^2 - 9*y^2 = 2 for some integer x.
 * https://oeis.org/A083043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=21, a(n)=20*a(n-1)-a(n-2) for n>2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, return 0 for safety */
        return (seq_t)0;
    if (n == 1)          /* first term */
        return (seq_t)1;
    if (n == 2)          /* second term */
        return (seq_t)21;

    idx_t i;
    seq_t prev2 = (seq_t)1;      /* a(1) */
    seq_t prev1 = (seq_t)21;     /* a(2) */
    seq_t curr;

    for (i = 3; i <= n; ++i) {
        curr = (seq_t)(20 * prev1) - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
