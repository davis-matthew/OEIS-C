/*
 * A025860  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^8)).
 * https://oeis.org/A025860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 32*n + 320 - 10*(n+8)*(n mod 4))/320)
 * Range:     u8 none, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t mod4 = (seq_t)(n % 4);
    seq_t num  = nn * nn + (seq_t)32 * nn + 320
                 - (seq_t)10 * (nn + 8) * mod4;
    return num / 320;
}
