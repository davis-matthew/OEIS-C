/*
 * A001148  Final digit of 3^n.
 * https://oeis.org/A001148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = last digit of 3^n; periodic with period 4
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static seq_t vals[] = {1, 3, 9, 7};
    return vals[n % 4];
}
