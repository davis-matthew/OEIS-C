/*
 * A004629  Squares written in base 11. (Next term contains a nondecimal
 *          digit.)
 * https://oeis.org/A004629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal interpretation of n² in base 11
 * Range:     u8 n <= 15, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t square = (seq_t)n * ((seq_t)n);
    seq_t result = 0;
    seq_t power_of_10 = 1;
    
    while (square > 0) {
        seq_t digit = square % 11;
        result = result + digit * power_of_10;
        power_of_10 = power_of_10 * 10;
        square = square / 11;
    }
    
    return result;
}
