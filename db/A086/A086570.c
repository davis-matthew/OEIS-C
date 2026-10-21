/*
 * A086570  Expansion of (1 + 3x + 5x^2 + 7x^3 + ...) / (1 - 2x + 3x^2 - 4x^3
 *          + ...).
 * https://oeis.org/A086570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A086570
   Formula: a(0) = 1, a(1) = 5, a(n) = 8*n - 4  for n >= 2   */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)5;
    /* n >= 2 */
    return (seq_t)8 * (seq_t)n - (seq_t)4;
}
