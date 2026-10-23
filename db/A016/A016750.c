/*
 * A016750  a(n) = (2*n)^10.
 * https://oeis.org/A016750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)^10
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 42, u128 n <= 3565,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to seq_t and compute base = 2*n */
    seq_t base = (seq_t)n * (seq_t)2;
    /* Compute base^10 by repeated multiplication */
    seq_t result = base;
    for (idx_t i = 1; i < 10; ++i) {
        result *= base;
    }
    return result;
}
