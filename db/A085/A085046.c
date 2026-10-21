/*
 * A085046  a(n) = n^2 - (1 + (-1)^n)/2.
 * https://oeis.org/A085046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - (1 + (-1)^n)/2
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic. */
    seq_t nn = (seq_t)n;
    /* Compute n^2. */
    seq_t val = nn * nn;
    /* Subtract 1 if n is even, 0 if n is odd. */
    if ((n & 1) == 0) {
        val -= 1;
    }
    return val;
}
