/*
 * A021867  Decimal expansion of 1/863.
 * https://oeis.org/A021867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n‑th digit after the decimal point of 1/863 (0‑based)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 1;   /* remainder in the division by 863 */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10;
        digit = r / 863;
        r = r % 863;
    }
    return digit;
}
