/*
 * A168182  Characteristic function of numbers that are not multiples of 9.
 * https://oeis.org/A168182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, mult, nonn
 * Formula:   a(n) = 1 if (n mod 9) ≠ 0 else 0
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return ((n % 9ULL) == 0) ? 0 : 1;
}
