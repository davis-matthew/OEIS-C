/*
 * A129428  Centered 47-gonal numbers.
 * https://oeis.org/A129428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 47*n*(n+1)/2 + 1
 * Range:     u8 n <= 2, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = nn * (nn + 1) / 2;  /* n*(n+1)/2 */
    return t * 47 + 1;
}
