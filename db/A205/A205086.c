/*
 * A205086  a(n) = n 6's sandwiched between two 1's.
 * https://oeis.org/A205086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 51 ; a(0) = 11
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1;                  /* start with the leading '1' */
    for (idx_t i = 0; i < n; ++i)
        x = x * 10 + 6;           /* append each '6' */
    x = x * 10 + 1;               /* append the final '1' */
    return x;
}
