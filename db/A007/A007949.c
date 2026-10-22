/*
 * A007949  Greatest k such that 3^k divides n. Or, 3-adic valuation of n.
 * https://oeis.org/A007949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n != 0 (mod 3), otherwise a(n) = 1 + a(n/3)
 * Range:     u8 n <= 345, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t nn = (seq_t)n;
    while (nn % 3 == 0) {
        count++;
        nn /= 3;
    }
    return count;
}
