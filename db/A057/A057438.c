/*
 * A057438  a(1) = 1; a(n+1) = (Product_{k = 1..n} [a(k)]) * (Sum_{j = 1..n}
 *          [1/a(j)]).
 * https://oeis.org/A057438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)^2 + a(n-1)*a(n-2) - a(n-2)^3 for n>3 Initial
 *            values: a(1)=1, a(2)=1, a(3)=2
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    seq_t prev2 = (seq_t)1;  /* a(2)   */
    seq_t prev1 = (seq_t)2;  /* a(3)   */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t sq   = prev1 * prev1;          /* a(n-1)^2     */
        seq_t prod = prev1 * prev2;          /* a(n-1)*a(n-2)*/
        seq_t cube = prev2 * prev2 * prev2;  /* a(n-2)^3     */
        seq_t cur  = sq + prod - cube;       /* a(n)          */

        prev2 = prev1;  /* shift for next iteration */
        prev1 = cur;
    }

    return prev1;
}
