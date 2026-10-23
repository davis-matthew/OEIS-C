/*
 * A021277  Decimal expansion of 1/273.
 * https://oeis.org/A021277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/273 after the
 *            decimal point. The digits repeat with period 6: 0, 0, 3, 6, 6, 3
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Periodic table of digits */
    static const seq_t cycle[6] = {0, 0, 3, 6, 6, 3};
    return cycle[n % 6];
}
