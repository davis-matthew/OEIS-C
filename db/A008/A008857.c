/*
 * A008857  a(n) = floor(n/9)*ceiling(n/9).
 * https://oeis.org/A008857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/9)*ceil(n/9)
 * Range:     u8 n <= 143, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t q = N / 9;       /* floor(n/9) */
    seq_t r = (N + 8) / 9; /* ceil(n/9) */
    return q * r;
}
