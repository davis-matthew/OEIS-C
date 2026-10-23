/*
 * A021523  Decimal expansion of 1/519.
 * https://oeis.org/A021523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of 1/519 (starting at a(0))
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* `rem` holds the current remainder, initially 1 = 1 mod 519. */
    idx_t rem = 1;
    seq_t digit = 0;

    /* Compute digits one by one up to the requested index. */
    for (idx_t i = 0; i <= n; ++i)
    {
        /* digit = (remainder * 10) / 519 */
        digit = (seq_t)(rem * 10) / 519;

        /* Update remainder: (remainder * 10) % 519 */
        rem = (idx_t)((rem * 10) % 519);
    }

    return digit;
}
