/*
 * A157820  a(n) = 27225*n^2 + 2*n.
 * https://oeis.org/A157820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27225*n^2 + 2*n
 * Range:     u8 none, u16 n <= 1, u32 n <= 397, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert to seq_t first to avoid any unintended integer promotion */
    seq_t tn = (seq_t)n;
    seq_t part = (seq_t)27225 * tn * tn;
    seq_t result = part + (seq_t)2 * tn;
    return result;
}
