/*
 * A017117  a(n) = (8*n + 4)^5.
 * https://oeis.org/A017117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 4)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 10, u64 n <= 890, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute x = 8*n + 4 */
    seq_t x = (seq_t)n * (seq_t)8 + (seq_t)4;

    /* Compute x^5 */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < (idx_t)5; ++i) {
        result *= x;
    }

    return result;
}
