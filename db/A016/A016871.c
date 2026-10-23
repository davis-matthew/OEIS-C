/*
 * A016871  a(n) = (5*n + 1)^11.
 * https://oeis.org/A016871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 1)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 11, u128 n <= 636,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute base = 5*n + 1, using seq_t arithmetic */
    seq_t base = (seq_t)5 * (seq_t)n + 1;
    seq_t result = 1;
    /* exponentiation by repeated multiplication (11 times) */
    for (int i = 0; i < 11; ++i) {
        result = result * base;
    }
    return result;
}
