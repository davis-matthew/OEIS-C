/*
 * A154997  a(n) = 6*a(n-1) + 30*a(n-2), n>2; a(0)=1, a(1)=1, a(2)=11.
 * https://oeis.org/A154997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 1, a(1) = 1, a(2) = 11 * a(n) = 6*a(n-1) + 30*a(n-2)
 *            for n > 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)11;

    /* a(n-2)  and  a(n-1) for the current step */
    seq_t prev2 = (seq_t)1;   /* a(1)   */
    seq_t prev1 = (seq_t)11;  /* a(2)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = 6 * prev1 + 30 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
