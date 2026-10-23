/*
 * A157879  Expansion of 120*x^2 / (-x^3+899*x^2-899*x+1).
 * https://oeis.org/A157879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0, a(2) = 120, a(3) = 107880 a(n) = 899*(a(n-1)-a(n-2)) +
 *            a(n-3) for n > 3
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)120;
    if (n == 3) return (seq_t)107880;

    seq_t x0 = (seq_t)0;          // a(n-3)
    seq_t x1 = (seq_t)120;        // a(n-2)
    seq_t x2 = (seq_t)107880;     // a(n-1)

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)899 * (x2 - x1) + x0;
        x0 = x1;
        x1 = x2;
        x2 = next;
    }
    return x2;
}
