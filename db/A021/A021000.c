/*
 * A021000  Duplicate of A020727.
 * https://oeis.org/A021000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)=2, a(1)=7, a(n)=4*a(n-1)-2*a(n-2)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)7;

    seq_t prev2 = (seq_t)2;   /* a(0) */
    seq_t prev1 = (seq_t)7;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)4 * prev1 - (seq_t)2 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
