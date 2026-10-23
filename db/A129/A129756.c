/*
 * A129756  Repetitions of odd numbers four times.
 * https://oeis.org/A129756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*floor(n/4)
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute floor(n/4), multiply by 2, and add 1 */
    return (seq_t)1 + (seq_t)2 * (seq_t)(n / 4);
}
