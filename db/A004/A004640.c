/*
 * A004640  Cubes written in base 11. (Next term contains a non-decimal
 *          character.)
 * https://oeis.org/A004640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is n^3 expressed in base 11, with base-11 digits
 *            interpreted as a decimal number
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;
    seq_t result = 0;
    seq_t power_of_10 = 1;
    
    while (cube > 0) {
        seq_t digit = cube % 11;
        result += digit * power_of_10;
        power_of_10 *= 10;
        cube /= 11;
    }
    
    return result;
}
