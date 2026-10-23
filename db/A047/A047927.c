/*
 * A047927  a(n) = n*(n-1)*(n-2)^2.
 * https://oeis.org/A047927
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)^2 (offset 2)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t nt  = (seq_t)n;
    seq_t nm1 = nt - 1;
    seq_t nm2 = nt - 2;
    return nt * nm1 * nm2 * nm2;
}
