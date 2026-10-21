/*
 * A093960  a(1) = 1, a(2) = 2, a(n+1) = n*a(1) + (n-1)*a(2) + ... +
 *          (n-r)*a(r+1) + ... + a(n).
 * https://oeis.org/A093960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=4, a(4)=11; a(n)=3*a(n-1)-a(n-2) for n>4
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)11;

    seq_t prev2 = (seq_t)4;   /* a(3) */
    seq_t prev1 = (seq_t)11;  /* a(4) */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        cur = (seq_t)3 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
