/*
 * A199022  a(n) = 5*11^n - 1.
 * https://oeis.org/A199022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*11^n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 11^n */
    seq_t power = 1;
    for (idx_t i = 0; i < n; ++i)
        power *= (seq_t)11;

    /* Multiply by 5 and subtract 1 */
    return power * (seq_t)5 - (seq_t)1;
}
