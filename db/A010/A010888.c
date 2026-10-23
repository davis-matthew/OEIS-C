/*
 * A010888  Digital root of n (repeatedly add the digits of n until a single
 *          digit is reached).
 * https://oeis.org/A010888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   a(n) = 0 if n = 0, otherwise a(n) = (n % 9 == 0 ? 9 : n % 9)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    seq_t r = (seq_t)(n % 9);
    return (r == 0) ? 9 : r;
}
