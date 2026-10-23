/*
 * A034829  a(n) = n-th sept-factorial number divided by 2.
 * https://oeis.org/A034829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product_{j=1..n} (7*j-5) / 2 Recurrence: a(1)=1,
 *            a(n)=a(n-1)*(7*n-5) for n>1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)          /* offset 1: a(1)=1 */
        return 1;

    seq_t res = 1;       /* value of a(1) */
    for (idx_t j = 2; j <= n; ++j)
        res *= (seq_t)(7 * j - 5);   /* multiply to obtain a(j) */

    return res;          /* res == a(n) */
}
