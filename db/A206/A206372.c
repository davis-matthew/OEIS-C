/*
 * A206372  a(n) = 14 * 4^n - 1.
 * https://oeis.org/A206372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14 * 4^n - 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow4 = 1;          /* start with 4^0 */
    for (idx_t i = 0; i < n; ++i) {
        pow4 = pow4 * 4;     /* multiply by 4 each step: 4^i -> 4^(i+1) */
    }
    pow4 = pow4 * 14;        /* 14 * 4^n */
    pow4 = pow4 - 1;         /* subtract 1 */
    return pow4;
}
