/*
 * A013813  a(n) = 20^(4*n + 3).
 * https://oeis.org/A013813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20^(4*n+3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponent: 4*n + 3 */
    idx_t e = n * 4 + 3;
    seq_t result = 1;
    for (idx_t i = 0; i < e; ++i) {
        result *= (seq_t)20;
    }
    return result;
}
