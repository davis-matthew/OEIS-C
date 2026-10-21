/*
 * A087289  a(n) = 2^(2*n+1) + 1.
 * https://oeis.org/A087289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(2*n + 1) + 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 3; a(n) = 4*a(n-1) - 3 (for n > 0) */

seq_t a(idx_t n)
{
    /* Handle the base case directly */
    if (n == 0)
        return (seq_t)3;

    /* Compute iteratively: a(n) = 4*a(n-1) - 3 */
    /* This avoids any need for power tables or large exponentiation
       and works uniformly for all supported integer types. */
    seq_t res = (seq_t)3;
    for (idx_t i = 1; i <= n; ++i)
    {
        res = res * (seq_t)4 - (seq_t)3;
    }
    return res;
}
