/*
 * A218749  a(n) = (46^n - 1)/45.
 * https://oeis.org/A218749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (46^n - 1)/45 = 46*a(n-1) + 1, a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset directly */
    if (n == 0)
        return (seq_t)0;

    seq_t val = (seq_t)0;
    /* iterate the linear recurrence a(k) = 46*a(k-1) + 1 */
    for (idx_t i = 0; i < n; ++i)
        val = (seq_t)46 * val + (seq_t)1;

    return val;
}
