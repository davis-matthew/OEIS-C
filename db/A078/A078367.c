/*
 * A078367  A Chebyshev T-sequence with Diophantine property.
 * https://oeis.org/A078367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 17*a(n-1) - a(n-2), a(-1)=17, a(0)=2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)17;

    seq_t prevprev = (seq_t)2;   /* a(0)   */
    seq_t prev     = (seq_t)17;  /* a(1)   */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)17 * prev - prevprev;
        prevprev = prev;
        prev     = cur;
    }
    return prev;
}
