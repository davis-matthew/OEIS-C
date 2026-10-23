/*
 * A016853  a(n) = (5*n)^5.
 * https://oeis.org/A016853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 16, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to seq_t and compute the base (5*n) */
    seq_t base = (seq_t)n * 5;

    /* Compute base^5 using a simple multiplication loop */
    seq_t result = 1;
    for (int i = 0; i < 5; i++) {
        result *= base;
    }

    return result;
}
