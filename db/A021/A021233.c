/*
 * A021233  Decimal expansion of 1/229.
 * https://oeis.org/A021233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit after decimal point of 1/229 (0-indexed)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    unsigned int rem = 1;
    unsigned int digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        digit = (rem * 10) / 229;
        rem = (rem * 10) % 229;
    }
    return (seq_t)digit;
}
