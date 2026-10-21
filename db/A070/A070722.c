/*
 * A070722  a(n) = n^7 mod 41.
 * https://oeis.org/A070722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^7) mod 41
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t mod = 41;
    seq_t n_mod = (seq_t)(n % 41);
    seq_t result = 1;
    for (int i = 0; i < 7; ++i) {
        result = (result * n_mod) % mod;
    }
    return result;
}
