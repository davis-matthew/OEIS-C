/*
 * A233325  a(n) = (2*6^(n+1) - 7) / 5.
 * https://oeis.org/A233325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 7, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = 1;        /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        value = value * 6 + 7;  /* recurrence */
    }
    return value;
}
