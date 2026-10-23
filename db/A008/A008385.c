/*
 * A008385  Coordination sequence for A_5 lattice.
 * https://oeis.org/A008385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (21*n^4 + 35*n^2 + 4)/2 for n >= 1, with a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 141, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t x  = (seq_t)n;      /* convert once to seq_t */
    seq_t n2 = x * x;
    seq_t n4 = n2 * n2;
    seq_t term = (seq_t)21 * n4 + (seq_t)35 * n2 + (seq_t)4;
    return term / 2;
}
