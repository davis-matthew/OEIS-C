/*
 * A073548  Number of Fibonacci numbers F(k), k <= 10^n, which end in 2.
 * https://oeis.org/A073548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 1 for n = 1, a(n) = a(n-1) * 10 + 6 for n >= 2 (i.e. the
 *            integer consisting of n-1 sixes)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* offset safety, though n >= 1 in practice */
        return (seq_t)0;

    /* n == 1 */
    if (n == 1)
        return (seq_t)1;

    /* For n >= 2 */
    seq_t res = (seq_t)6;           /* a(2) */
    for (idx_t i = 3; i <= n; ++i) /* iterate up to n, multiplying by 10 and adding 6 each step */
        res = res * (seq_t)10 + (seq_t)6;

    return res;
}
