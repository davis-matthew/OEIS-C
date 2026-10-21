/*
 * A207450  Number of n X 5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 48*n^3 - 32*n^2
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* n as seq_t               */
    seq_t n2 = nn * nn;           /* n^2                      */
    seq_t n3 = n2 * nn;           /* n^3                      */
    return (seq_t)48 * n3 - (seq_t)32 * n2; /* 48*n^3 - 32*n^2 */
}
