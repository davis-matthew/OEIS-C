/*
 * A141032  a(n) = 4*(16^n - 1)/15.
 * https://oeis.org/A141032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(16^n - 1)/15
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 4, a(n) = 16*a(n-1) + 4 */

seq_t a(idx_t n)
{
    seq_t value = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute a(i) = 16*a(i-1) + 4 */
        value = value * 16 + 4;
    }
    return value;
}
