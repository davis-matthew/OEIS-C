/*
 * A038867  a(n) = (n+5)^3 - n^3.
 * https://oeis.org/A038867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5)^3 - n^3 = 5*(3*n^2 + 15*n + 25)
 * Range:     u8 n <= 1, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t res = sn * sn;       /* n^2 */
    res = res * 3;             /* 3*n^2 */
    res += sn * 15;            /* +15*n */
    res += 25;                 /* +25 */
    res *= 5;                  /* multiply by 5 */
    return res;
}
