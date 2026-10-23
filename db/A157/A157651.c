/*
 * A157651  a(n) = 100*n^2 - 49*n + 6.
 * https://oeis.org/A157651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*n^2 - 49*n + 6
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 6553, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t r = (seq_t)100 * nn * nn;
    r -= (seq_t)49 * nn;
    r += 6;
    return r;
}
