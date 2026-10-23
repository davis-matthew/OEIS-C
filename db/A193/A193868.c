/*
 * A193868  Even central polygonal numbers.
 * https://oeis.org/A193868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2-3*n+2 if n is even, 2*n^2-n+1 if n is odd
 * Range:     u8 n <= 12, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = n;
    if ((n_t & 1) == 0) {
        return 2 * n_t * n_t - 3 * n_t + 2;
    } else {
        return 2 * n_t * n_t - n_t + 1;
    }
}
