/*
 * A063092  a(0)=1, a(1)=2 and, for n>1, a(n) = a(n-1) + 11*a(n-2).
 * https://oeis.org/A063092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 11*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t prev2 = (seq_t)1;   /* a(0)  */
    seq_t prev1 = (seq_t)2;   /* a(1)  */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t curr = prev1 + (seq_t)11 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
