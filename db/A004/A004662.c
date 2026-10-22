/*
 * A004662  Powers of 3 written in base 8.
 * https://oeis.org/A004662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3^n written in base 8 (octal digits read as a decimal
 *            number)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p = 1;
    idx_t i;
    for (i = 0; i < n; i++)
        p = p * 3;

    seq_t result = 0;
    seq_t multiplier = 1;
    while (p > 0) {
        seq_t digit = p % 8;
        result = result + digit * multiplier;
        multiplier = multiplier * 10;
        p = p / 8;
    }
    return result;
}
