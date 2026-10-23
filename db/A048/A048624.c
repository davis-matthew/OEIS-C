/*
 * A048624  Essentially a duplicate of A000129.
 * https://oeis.org/A048624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2*a(n-1) + a(n-2), a(0) = 2, a(1) = 5
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)5;

    seq_t prev2 = (seq_t)2;  /* a(0) */
    seq_t prev1 = (seq_t)5;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)2 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
