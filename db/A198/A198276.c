/*
 * A198276  a(n) = 19*2^n-1.
 * https://oeis.org/A198276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*2^n - 1 Recurrence: a(0) = 18, a(n+1) = 2*a(n) + 1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 18;                /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        v = v * 2 + 1;           /* a(i) = 2*a(i-1) + 1 */
    }
    return v;
}
