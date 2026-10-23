/*
 * A130196  Period 3: repeat [1, 2, 2].
 * https://oeis.org/A130196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 mod 3) + 1 (periodic pattern 1, 2, 2)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Determine n modulo 3 */
    idx_t r = n % 3;
    /* Return 1 if r == 0, else 2 */
    return (r == 0) ? (seq_t)1 : (seq_t)2;
}
