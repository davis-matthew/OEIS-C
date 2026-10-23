/*
 * A177353  n! (mod n^2+1).
 * https://oeis.org/A177353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! mod (n^2+1)
 * Range:     u8 n <= 15, u16 n <= 280, u32 n <= 2984, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t mod = ((seq_t)n) * ((seq_t)n) + 1;
    for (idx_t i = 1; i <= n; ++i) {
        result = (result * (seq_t)i) % mod;
    }
    return result;
}
