/*
 * A017410  a(n) = (11*n + 1)^10.
 * https://oeis.org/A017410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 1)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 7, u128 n <= 648,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before multiplication */
    seq_t base = (seq_t)n * 11 + 1;
    seq_t result = 1;

    /* Compute the 10th power by repeated multiplication */
    for (idx_t i = 0; i < 10; ++i) {
        result *= base;
    }
    return result;
}
