/*
 * A069473  First differences of (n+1)^6-n^6 (A022522).
 * https://oeis.org/A069473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*n^4 + 120*n^3 + 210*n^2 + 180*n + 62
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t x2 = x * x;           /* x^2 */
    seq_t x3 = x2 * x;          /* x^3 */
    seq_t x4 = x3 * x;          /* x^4 */

    seq_t result = ((seq_t)30) * x4;
    result += ((seq_t)120) * x3;
    result += ((seq_t)210) * x2;
    result += ((seq_t)180) * x;
    result += ((seq_t)62);

    return result;
}
