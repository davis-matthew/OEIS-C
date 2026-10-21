/*
 * A229689  Number of defective 3-colorings of an n X 3 0..2 array connected
 *          horizontally and antidiagonally with exactly two mistakes, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 108*a(n-2) + 216*a(n-3) (n > 4) with a(1)=1,
 *            a(2)=43, a(3)=626, a(4)=6732
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)43;
    if (n == 3) return (seq_t)626;
    if (n == 4) return (seq_t)6732;

    /* window for n = current index: a(n-3), a(n-2), a(n-1)  */
    seq_t prev3 = (seq_t)43;     /* a(2)   */
    seq_t prev2 = (seq_t)626;    /* a(3)   */
    seq_t prev1 = (seq_t)6732;   /* a(4)   */
    seq_t cur   = 0;

    for (idx_t i = 5; i <= n; ++i) {
        cur = (seq_t)18 * prev1
             - (seq_t)108 * prev2
             + (seq_t)216 * prev3;
        /* shift window for next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
