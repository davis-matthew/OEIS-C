/*
 * A070725  a(n) = n^7 mod 45.
 * https://oeis.org/A070725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 45
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t mod = (seq_t)45;
    seq_t x = (seq_t)n % mod;
    seq_t result = 1;
    for (int i = 0; i < 7; ++i) {
        result = (result * x) % mod;
    }
    return result;
}
