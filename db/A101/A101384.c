/*
 * A101384  a(n) = n*(n-1)^3*(n^2-n-1)/2.
 * https://oeis.org/A101384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)^3*(n^2-n-1)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t sn = n;
    seq_t n1 = sn - 1;
    seq_t n1_sq = n1 * n1;
    seq_t n1_cu = n1_sq * n1;        /* (n-1)^3 */
    seq_t temp = sn * n1_cu;         /* n*(n-1)^3 */
    temp = temp / 2;                 /* divide by 2 */
    seq_t n2 = sn * sn - sn - 1;     /* n^2-n-1 */
    return temp * n2;
}
