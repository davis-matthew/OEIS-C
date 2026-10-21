/*
 * A084058  a(n) = 2*a(n-1) + 7*a(n-2) for n>1, a(0)=1, a(1)=1.
 * https://oeis.org/A084058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t cur   = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 2 * prev1 + 7 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
