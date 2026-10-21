/*
 * A052379  Number of integers from 1 to 10^(n+1)-1 that lack 0 and 1 as a
 *          digit.
 * https://oeis.org/A052379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 8
 *          a(n) = 8 * a(n-1) + 8   for n ≥ 1
 * This recurrence yields a(n) = (8^(n+2) - 1)/7 - 1.
 */
seq_t a(idx_t n)
{
    seq_t result = 8;              /* a(0) */
    for (idx_t i = 1; i <= n; ++i) /* compute up to a(n) */
    {
        result = result * 8 + 8;
    }
    return result;
}
