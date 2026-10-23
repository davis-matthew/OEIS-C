/*
 * A133893  Numbers m such that binomial(m+3,m) mod 3 = 0.
 * https://oeis.org/A133893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n + 6 - 2*(n mod 3)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t mod3 = nn % 3;
    return 3 * nn + 6 - 2 * mod3;
}
