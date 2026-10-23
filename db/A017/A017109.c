/*
 * A017109  a(n) = (8*n+3)^9.
 * https://oeis.org/A017109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+3)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 16, u128 n <= 2388,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t for arithmetic in seq_t type */
    seq_t nn = (seq_t)n;
    /* Compute the base: 8*n + 3 */
    seq_t base = nn * (seq_t)8 + (seq_t)3;

    /* Compute base^9 by repeated multiplication */
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < (idx_t)9; ++i) {
        res *= base;
    }
    return res;
}
