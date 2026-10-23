/*
 * A013912  a(n) = 24^(5*n + 3).
 * https://oeis.org/A013912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24^(5*n+3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = (seq_t)13824;          /* 24^3 */
    const seq_t base = (seq_t)7962624;   /* 24^5 */
    for (idx_t i = 0; i < n; ++i)
        value = value * base;
    return value;
}
