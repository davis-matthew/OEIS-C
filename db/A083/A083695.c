/*
 * A083695  a(n) = 2*a(n-1) + 5*a(n-2), with a(0) = 0, a(1) = 3.
 * https://oeis.org/A083695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n‑1) + 5*a(n‑2); a(0) = 0, a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* a(0) */
    if (n == (idx_t)1)
        return (seq_t)3;          /* a(1) */

    seq_t prev2 = (seq_t)0;       /* a(0) */
    seq_t prev1 = (seq_t)3;       /* a(1) */
    seq_t curr;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        curr = (seq_t)2 * prev1 + (seq_t)5 * prev2;  /* a(i) = 2*a(i-1)+5*a(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;                 /* a(n) */
}
