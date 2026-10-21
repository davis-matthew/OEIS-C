/*
 * A086576  a(n) = 5*(10^n - 1).
 * https://oeis.org/A086576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5 * (10^n - 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow10 = 1;                     /* pow10 = 10^0 */
    for (idx_t i = 0; i < n; ++i)        /* multiply by 10 n times */
        pow10 *= (seq_t)10;
    return pow10 * (seq_t)5 - (seq_t)5;  /* 5*(10^n) - 5 = 5*(10^n-1) */
}
