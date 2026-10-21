/*
 * A080874  a(n)*a(n+3) - a(n+1)*a(n+2) = 5, given a(0)=a(1)=1, a(2)=3.
 * https://oeis.org/A080874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-2) - a(n-4), with a(0)=1, a(1)=1, a(2)=3, a(3)=8
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)8;

    seq_t prev4 = (seq_t)1;  /* a(0) */
    seq_t prev3 = (seq_t)1;  /* a(1) */
    seq_t prev2 = (seq_t)3;  /* a(2) */
    seq_t prev1 = (seq_t)8;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = 10 * prev2 - prev4;  /* a(i) = 10*a(i-2) - a(i-4) */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
