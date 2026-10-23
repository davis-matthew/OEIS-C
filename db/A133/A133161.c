/*
 * A133161  Indices of the triangular numbers which are also centered
 *          triangular number.
 * https://oeis.org/A133161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 1; a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset (a(1)=1, a(2)=4) */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* Iteratively apply the recurrence; O(n) time, O(1) space. */
    seq_t prevprev = (seq_t)1;   /* a(1) */
    seq_t prev     = (seq_t)4;   /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)4 * prev - prevprev + 1;
        prevprev = prev;
        prev = cur;
    }
    return prev;   /* a(n) */
}
