/*
 * A078008  Expansion of (1 - x)/((1 + x)*(1 - 2*x)).
 * https://oeis.org/A078008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2), a(0) = 1, a(1) = 0
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t a0 = (seq_t)1;   /* a(n-2)  */
    seq_t a1 = (seq_t)0;   /* a(n-1)  */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = a1 + a0 + a0;   /* cur = a(i-1) + 2*a(i-2) */
        a0 = a1;              /* shift window forward */
        a1 = cur;
    }
    return cur;
}
