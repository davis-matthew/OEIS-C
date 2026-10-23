/*
 * A016195  a(n) = 11^n - 10^n.
 * https://oeis.org/A016195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 10^n Recurrence: a(0) = 0 a(n) = 11*a(n-1) +
 *            10^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow10  = 1;           /* 10^(i-1) for the current i   */
    for (idx_t i = 1; i <= n; ++i) {
        result = result * (seq_t)11 + pow10; /* a(i) = 11*a(i-1) + 10^(i-1) */
        pow10 *= (seq_t)10;                     /* next power of 10      */
    }
    return result;
}
