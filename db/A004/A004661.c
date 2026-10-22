/*
 * A004661  Powers of 3 written in base 7.
 * https://oeis.org/A004661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007093(3^n), i.e. 3^n written in base 7 and read as a
 *            decimal number
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t i;
    for (i = 0; i < n; i++)
        v *= 3;

    seq_t result = 0;
    seq_t place = 1;
    while (v > 0) {
        seq_t digit = v % 7;
        result += digit * place;
        place *= 10;
        v /= 7;
    }
    return result;
}
