/*
 * A123012  Expansion of 1/(1 - 2*x - 21*x^2).
 * https://oeis.org/A123012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 21*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values */
    const seq_t A0 = 1;
    const seq_t A1 = 2;

    if (n == 0) return A0;
    if (n == 1) return A1;

    seq_t prevprev = A0;  /* a(n-2) */
    seq_t prev     = A1;  /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* cur = 2*prev + 21*prevprev */
        cur = (seq_t)2 * prev + (seq_t)21 * prevprev;
        /* shift for next iteration */
        prevprev = prev;
        prev     = cur;
    }
    return cur;
}
