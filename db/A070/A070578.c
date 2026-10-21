/*
 * A070578  a(n) = n^4 mod 35.
 * https://oeis.org/A070578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 35
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mod = 35;
    seq_t x   = (seq_t)n % mod;
    seq_t x2  = (x * x) % mod;
    seq_t x3  = (x2 * x) % mod;
    seq_t x4  = (x3 * x) % mod;
    return x4;
}
