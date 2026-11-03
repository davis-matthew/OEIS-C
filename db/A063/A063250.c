/*
 * A063250  Number of binary right-rotations (iterations of A038572) to reach
 *          fixed point.
 * https://oeis.org/A063250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 0 if n+1 is a power of 2; otherwise a(n) = 1 +
 *            a(floor(n/2))
 * Range:     u8 n <= 32767, u16 n <= 32767, u32 n <= 32767, u64 n <= 32767,
 *            u128 n <= 32767, big any n
 * Validated: standard, pass, 121 terms through n = 6002 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 0;
    while (((n + 1) & n) != 0) {
        ++res;
        n >>= 1;
    }
    return res;
}
