/*
 * A157456  Expansion of x * (1 - x) / (1 - 16*x + x^2).
 * https://oeis.org/A157456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 15; a(n) = 16*a(n-1) - a(n-2) (n ≥ 3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* undefined for n = 0, but return zero for safety */
    if (n == 0) return (seq_t)0;

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)15;

    seq_t prev2 = (seq_t)1;   /* a(1)   */
    seq_t prev1 = (seq_t)15;  /* a(2)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 * (seq_t)16 - prev2;  /* a(i) = 16*a(i-1) - a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
