/*
 * A199833  Number of -n..n arrays of 4 elements with zero sum and no two
 *          neighbors summing to zero.
 * https://oeis.org/A199833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (16*n^3 - 4*n)/3
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    seq_t t  = (seq_t)16 * nt * nt * nt - (seq_t)4 * nt;
    return t / 3;
}
