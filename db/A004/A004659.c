/*
 * A004659  Powers of 3 written in base 5.
 * https://oeis.org/A004659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3^n written in base 5 (base-5 digits of 3^n read as a
 *            decimal number)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t power = 1;
    idx_t i;
    for (i = 0; i < n; i++)
        power = power * 3;

    seq_t result = 0;
    seq_t multiplier = 1;

    while (power > 0) {
        seq_t digit = power % 5;
        result = result + digit * multiplier;
        power = power / 5;
        if (power > 0)
            multiplier = multiplier * 10;
    }

    return result;
}
