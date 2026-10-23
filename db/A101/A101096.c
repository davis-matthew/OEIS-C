/*
 * A101096  Third differences of fifth powers (A000584).
 * https://oeis.org/A101096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(2) = 29, a(n) = 30*(2*(n-1)^2 - 2*(n-1) + 1) for n
 *            >= 3 Equivalently: a(n) = 60*(n-1)^2 - 60*(n-1) + 30 (n >= 3)
 * Range:     u8 n <= 3, u16 n <= 34, u32 n <= 8462, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 29;
    seq_t m = (seq_t)(n - 1);     /* m = n-1  (n >= 3) */
    seq_t res = 60 * m * m;       /* 60*m^2 */
    res -= 60 * m;                /* -60*m */
    res += 30;                    /* +30 */
    return res;
}
