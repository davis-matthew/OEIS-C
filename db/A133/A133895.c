/*
 * A133895  Numbers m such that binomial(m+5,m) mod 5 = 0.
 * https://oeis.org/A133895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n + 20 - 4*(n mod 5)
 * Range:     u8 n <= 49, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert n to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t rem = nn % 5U;
    seq_t result = 5U * nn + 20U - 4U * rem;
    return result;
}
