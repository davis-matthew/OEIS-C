/*
 * A013823  a(n) = 2^(5*n + 2).
 * https://oeis.org/A013823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(5*n + 2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t exp = 5*n + 2;
    seq_t res = 1;
    while (exp--) res *= 2;
    return res;
}
