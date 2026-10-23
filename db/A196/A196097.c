/*
 * A196097  Boundary of an n-ball in Novikov's hyperbolic triangular
 *          discretization of complex analysis.
 * https://oeis.org/A196097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) for n >= 3, with a(0) = 1, a(1) = 8,
 *            a(2) = 32
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)8;
    if (n == 2u) return (seq_t)32;

    seq_t prev2 = (seq_t)8;   /* a(1) */
    seq_t prev1 = (seq_t)32;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 * (seq_t)4 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
