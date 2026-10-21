/*
 * A070498  a(n) = n^3 mod 36.
 * https://oeis.org/A070498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 36
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t mod = 36;
    seq_t nmod = (seq_t)(n % mod);
    seq_t r = (nmod * nmod) % mod;
    r = (r * nmod) % mod;
    return r;
}
