/*
 * A070505  a(n) = n^3 mod 43.
 * https://oeis.org/A070505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 43
 * Range:     u8 n <= 295, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)43;
    seq_t nmod = (seq_t)n % MOD;
    seq_t res = (nmod * nmod) % MOD;
    res = (res * nmod) % MOD;
    return res;
}
