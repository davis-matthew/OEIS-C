/*
 * A158639  a(n) = 676*n^2 - 26.
 * https://oeis.org/A158639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 676*n^2 - 26
 * Range:     u8 none, u16 n <= 9, u32 n <= 2520, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 for safety */
    seq_t t = (seq_t)n * (seq_t)n; /* n^2 */
    t *= ((seq_t)676);             /* 676*n^2 */
    t -= ((seq_t)26);              /* 676*n^2 - 26 */
    return t;
}
