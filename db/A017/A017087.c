/*
 * A017087  a(n) = (8*n + 1)^11.
 * https://oeis.org/A017087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 1)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 6, u128 n <= 397,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 8*n + 1 in seq_t to avoid overflow of idx_t. */
    seq_t base = ((seq_t)n * (seq_t)8) + (seq_t)1;

    /* Raise base to the 11th power by repeated multiplication. */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < 11; ++i) {
        result *= base;
    }
    return result;
}
