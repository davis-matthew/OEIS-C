/*
 * A120354  a(n) = 11*3^n.
 * https://oeis.org/A120354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 * 3^n
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Start with a(0) = 11 and repeatedly multiply by 3. */
    seq_t result = 11;
    for (idx_t i = 0; i < n; ++i)
        result *= 3;
    return result;
}
