/*
 * A052595  E.g.f. 1/(1-3x-x^2).
 * https://oeis.org/A052595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A052595
 *
 *  Exponential generating function: 1 / (1 - 3x - x²)
 *  a(0) = 1
 *  a(1) = 3
 *  Recurrence (valid for n ≥ 0):
 *        a(n+2) = (2 + n² + 3n) * a(n) + (6 + 3n) * a(n+1)
 *
 *  Algorithm:  iterate the recurrence from i = 2 up to the required n.
 *  Only integer arithmetic is used; the sequence is computed exactly
 *  for any size that fits in the chosen seq_t type.
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)1;   /* a(i-2)   */
    seq_t prev1 = (seq_t)3;   /* a(i-1)   */
    seq_t cur   = (seq_t)0;   /* a(i)     */

    for (idx_t i = 2; i <= n; ++i) {
        idx_t m = i - 2;          /* current n in the recurrence */
        seq_t factor1 = (seq_t)(2 + m * m + (seq_t)3 * m);
        seq_t factor2 = (seq_t)(6 + (seq_t)3 * m);

        cur = factor1 * prev2 + factor2 * prev1;

        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
