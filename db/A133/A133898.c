/*
 * A133898  Numbers m such that binomial(m+8,m) mod 8 = 0.
 * https://oeis.org/A133898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n + 56 - 7*(n mod 8)
 * Range:     u8 n <= 31, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n mod 8 can be computed with bitwise AND for speed */
    idx_t r = n & 7U;          /* r = n % 8 */

    /* Construct the value using only seq_t arithmetic */
    return ((seq_t)n) * 8 + 56 - ((seq_t)7 * r);
}
