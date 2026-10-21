/*
 * A070488  a(n) = n^3 mod 26.
 * https://oeis.org/A070488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 26
 * Range:     u8 n <= 338, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 26;
    seq_t x = ((seq_t)n) % MOD;   // reduce n modulo 26
    seq_t y = (x * x) % MOD;      // x^2 mod 26
    seq_t z = (y * x) % MOD;      // x^3 mod 26
    return z;
}
