/*
 * A133897  Numbers m such that binomial(m+7,m) mod 7 = 0.
 * https://oeis.org/A133897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n + 42 - 6*(n mod 7)
 * Range:     u8 n <= 34, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t mod = n % 7;
    seq_t tn   = (seq_t)n;
    seq_t smod = (seq_t)mod;
    return (seq_t)7 * tn + (seq_t)42 - (seq_t)6 * smod;
}
