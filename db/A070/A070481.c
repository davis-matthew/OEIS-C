/*
 * A070481  a(n) = n^3 mod 19.
 * https://oeis.org/A070481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 19
 * Range:     u8 n <= 268, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nm = (seq_t)n % 19;
    seq_t res = (nm * nm) % 19;
    res = (res * nm) % 19;
    return res;
}
