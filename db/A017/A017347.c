/*
 * A017347  a(n) = (10*n + 6)^7.
 * https://oeis.org/A017347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 6)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 55, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base 10*n + 6 in seq_t */
    seq_t base = (seq_t)10 * n + (seq_t)6;

    /* Raise it to the 7th power */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 7; ++i) {
        result *= base;
    }
    return result;
}
