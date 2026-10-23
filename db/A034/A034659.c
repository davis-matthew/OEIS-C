/*
 * A034659  a(n) = (11^n + 1)/2.
 * https://oeis.org/A034659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11^n + 1)/2 (recurrence a(0)=1, a(n)=11*a(n-1)-5)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;             /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
        result = result * 11 - 5; /* recurrence */
    return result;
}
