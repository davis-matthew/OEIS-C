/*
 * A094434  a(n) = rightmost term of M^n * [1 0 0], with M = the 3 X 3 matrix
 *          [1 -1 0 / -1 3 -2 / 0 -2 2].
 * https://oeis.org/A094434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) Initial values: a(1) = 0, a(2) = 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    seq_t prev2 = (seq_t)0;  /* a(1) */
    seq_t prev1 = (seq_t)2;  /* a(2) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur = 6 * prev1 - 6 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
