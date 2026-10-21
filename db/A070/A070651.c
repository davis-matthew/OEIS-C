/*
 * A070651  a(n) = n^6 mod 28.
 * https://oeis.org/A070651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 28
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mod = (seq_t)28;
    seq_t base = (seq_t)n % mod;   /* n reduced modulo 28 first */
    seq_t res = (seq_t)1;
    for (int i = 0; i < 6; ++i)
        res = (res * base) % mod;
    return res;
}
