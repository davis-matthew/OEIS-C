/*
 * A121060  Sum of the first n digits to the right of the decimal expansion of
 *          1/n.
 * https://oeis.org/A121060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the first n decimal digits of 1/n after the
 *            decimal point
 * Range:     u8 n <= 58, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t rem = 1 % n;
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        unsigned __int128 prod = (unsigned __int128)rem * 10;
        idx_t digit = (idx_t)(prod / n);
        rem = (idx_t)(prod % n);
        sum += (seq_t)digit;
    }
    return sum;
}
