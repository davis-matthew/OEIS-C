/*
 * A059290  a(n) = round(1/144*n^2*(n + 3)).
 * https://oeis.org/A059290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round( (n^2 * (n + 3)) / 144 )
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn * (nn + (seq_t)3);
    seq_t res = (num + (seq_t)72) / (seq_t)144;   /* round by adding 0.5*144 */
    return res;
}
