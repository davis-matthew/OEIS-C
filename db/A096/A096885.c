/*
 * A096885  Related to diagonals of Pascal's triangle.
 * https://oeis.org/A096885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 100 * a(n-1) + a(n-2) with a(0) = 1, a(1) = 100
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A096885 for any n ≥ 0 using a linear recurrence. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)100;

    seq_t prev2 = 1;   /* a(n-2) */
    seq_t prev1 = 100; /* a(n-1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 * (seq_t)100 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
