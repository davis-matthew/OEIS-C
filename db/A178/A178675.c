/*
 * A178675  a(n) = 4^n + 4.
 * https://oeis.org/A178675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)5;          /* 4^0 + 4 = 5 */

    seq_t val = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        val *= (seq_t)4;          /* val = 4^n */

    return val + (seq_t)4;        /* 4^n + 4 */
}
