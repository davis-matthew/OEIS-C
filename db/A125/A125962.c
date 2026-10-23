/*
 * A125962  Numbers whose base-9 representation is 555555555......5.
 * https://oeis.org/A125962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(0) = 0, a(n) = 9*a(n-1) + 5 (n > 0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;          /* a(0) */
    for (idx_t i = 0; i < n; ++i)
    {
        result = result * (seq_t)9 + (seq_t)5;   /* recurrence */
    }
    return result;
}
