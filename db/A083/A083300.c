/*
 * A083300  a(n) = (4*7^n + (-3)^n)/5.
 * https://oeis.org/A083300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*7^n + (-3)^n)/5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p7 = 1;  /* 7^0 */
    seq_t p3 = 1;  /* 3^0 */
    for (idx_t i = 0; i < n; ++i) {
        p7 *= 7;
        p3 *= 3;
    }
    seq_t term = 4 * p7;   /* 4*7^n */
    if (n & 1) {
        term -= p3;          /* n odd => (-3)^n = -3^n */
    } else {
        term += p3;          /* n even => (-3)^n = 3^n */
    }
    return term / 5;        /* division by 5 is exact */
}
