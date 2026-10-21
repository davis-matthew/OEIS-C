/*
 * A070657  a(n) = n^6 mod 34.
 * https://oeis.org/A070657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 34
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t mod = 34;
    seq_t base = (seq_t)n % mod;      /* reduce first to avoid large intermediates */
    seq_t result = 1;
    for (int i = 0; i < 6; ++i)
        result = (result * base) % mod;
    return result;
}
