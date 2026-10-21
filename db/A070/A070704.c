/*
 * A070704  a(n) = n^7 mod 23.
 * https://oeis.org/A070704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 23
 * Range:     u8 n <= 304, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    seq_t base = (seq_t)n;
    const seq_t MOD = 23;
    for (int i = 0; i < 7; ++i)
        res = (res * base) % MOD;
    return res;
}
