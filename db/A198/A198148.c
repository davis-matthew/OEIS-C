/*
 * A198148  a(n) = n*(n+2)*(9 - 7*(-1)^n)/16.
 * https://oeis.org/A198148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+2)*(9 - 7*(-1)^n)/16
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {               /* n = 2*k  */
        seq_t k = (seq_t)(n >> 1);    /* k = n/2 */
        return k * (k + 1) / 2;
    } else {                          /* n = 2*k+1 */
        seq_t nn = (seq_t)n;
        return nn * (nn + 2);
    }
}
