/*
 * A017159  a(n) = (8*n + 7)^11.
 * https://oeis.org/A017159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 7)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 6, u128 n <= 397,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 8*n + 7. */
    seq_t base = (seq_t)n * (seq_t)8 + (seq_t)7;

    /* Raise base to the 11th power using repeated multiplication. */
    seq_t result = base;
    for (int i = 1; i < 11; ++i) {
        result *= base;
    }
    return result;
}
