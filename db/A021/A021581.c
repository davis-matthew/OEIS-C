/*
 * A021581  Decimal expansion of 1/577.
 * https://oeis.org/A021581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth digit after the decimal point in the decimal
 *            expansion of 1/577
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t rem = 1;          /* current remainder in the long division */
    seq_t digit = 0;        /* last computed digit */

    for (idx_t i = 0; i <= n; ++i) {
        rem = rem * (seq_t)10;          /* bring down next digit */
        digit = rem / (seq_t)577;       /* quotient is the next digit */
        rem = rem % (seq_t)577;         /* new remainder for next step */
    }
    return digit;
}
