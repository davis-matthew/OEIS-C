/*
 * A153448  3 times 12-gonal (or dodecagonal) numbers: a(n) = 3*n*(5*n-4).
 * https://oeis.org/A153448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n^2 - 12*n = 3*n*(5*n-4)
 * Range:     u8 n <= 4, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    /* Compute as 15*n^2 - 12*n to avoid underflow issues with unsigned arithmetic */
    seq_t res = 15 * sn * sn - 12 * sn;
    return res;
}
