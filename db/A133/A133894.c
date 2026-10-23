/*
 * A133894  Numbers m such that binomial(m+4,m) mod 4 = 0.
 * https://oeis.org/A133894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n + 8 - 3*((n-1) mod 4)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* r = (n-1) mod 4 */
    idx_t r = (n - 1) & 3ULL;
    return (seq_t)4 * (seq_t)n + (seq_t)8 - (seq_t)3 * (seq_t)r;
}
