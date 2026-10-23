/*
 * A179558  a(1)=10; a(n) = a(n-1)*10 - 5^(n-2).
 * https://oeis.org/A179558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=10, a(2)=99, a(n)=15*a(n-1)-50*a(n-2) for n>2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)99;

    seq_t prev2 = (seq_t)10;   /* a(n-2)   */
    seq_t prev1 = (seq_t)99;   /* a(n-1)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 * (seq_t)15 - prev2 * (seq_t)50;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
