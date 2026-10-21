/*
 * A212836  Number of 0..n arrays of length 3 with 0 never adjacent to n.
 * https://oeis.org/A212836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 + 3*n^2 - n - 1
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;
    seq_t inside = s * s + 3 * s - 1;
    seq_t res = s * inside - 1;
    return res;
}
