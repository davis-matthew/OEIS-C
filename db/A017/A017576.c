/*
 * A017576  a(n) = (12*n + 4)^8.
 * https://oeis.org/A017576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 4)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 20, u128 n <= 5460,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base 12*n + 4 */
    seq_t base = (seq_t)12 * n + (seq_t)4;

    /* Raise to the 8th power by repeated multiplication */
    seq_t result = (seq_t)1;
    for (unsigned int i = 0; i < 8; ++i) {
        result *= base;
    }

    return result;
}
