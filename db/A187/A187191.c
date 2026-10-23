/*
 * A187191  Number of 4-turn rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*n^5 - 34*n^4 + 48*n^3 - 22*n^2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;          /* n^2 */
    seq_t n3 = n2 * tn;          /* n^3 */
    seq_t n4 = n3 * tn;          /* n^4 */
    seq_t n5 = n4 * tn;          /* n^5 */

    /* Compute left = 8*n^5 + 48*n^3 and right = 34*n^4 + 22*n^2 */
    seq_t left  = 8 * n5;
    left += 48 * n3;

    seq_t right = 34 * n4;
    right += 22 * n2;

    /* left >= right for all n >= 1, so subtraction is safe */
    return left - right;
}
