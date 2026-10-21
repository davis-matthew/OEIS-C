/*
 * A057069  a(n) = floor(7^7/n).
 * https://oeis.org/A057069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(7^7 / n)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety guard, though offset is 1 */
    const seq_t const_val = (seq_t)823543;   /* 7^7 */
    return const_val / (seq_t)n;
}
