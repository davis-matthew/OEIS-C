/*
 * A207449  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*n^3 + 10*n^2 - 10*n
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t sn = (seq_t)n;
    seq_t res = (seq_t)10 * sn * sn * sn
              + (seq_t)10 * sn * sn
              - (seq_t)10 * sn;
    return res;
}
