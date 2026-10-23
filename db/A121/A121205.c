/*
 * A121205  "666" in bases 7 and higher rewritten in base 10.
 * https://oeis.org/A121205
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*(n^2 + n + 1)
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t tmp = nn * (nn + 1);  /* n * (n + 1) */
    tmp += 1;                   /* n*(n+1) + 1 */
    tmp *= 6;                   /* 6 * (n^2 + n + 1) */
    return tmp;
}
