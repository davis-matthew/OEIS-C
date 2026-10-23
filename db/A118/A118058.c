/*
 * A118058  a(n) = 49n^2 - 28n - 20.
 * https://oeis.org/A118058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49n^2 - 28n - 20 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute the quadratic polynomial on seq_t arithmetic */
    seq_t result = (seq_t)49 * nn * nn;
    result = result - (seq_t)28 * nn;
    result = result - (seq_t)20;
    return result;
}
