/*
 * A036987  Fredholm-Rueppel sequence.
 * https://oeis.org/A036987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if (n+1) is a power of two, otherwise 0
 * Range:     u8 n <= 9964, u16 n <= 9964, u32 n <= 9964, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n + 1;           /* x = n+1, careful for overflow */
    if (x == 0)                /* overflow case: n == UINT64_MAX */
        return (seq_t)0;
    return (seq_t)(((x & (x - 1)) == 0) ? 1 : 0);
}
