/*
 * A004669  Powers of 3 written in base 27.
 * https://oeis.org/A004669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 3^(n mod 3) * 10^floor(n/3)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t power_of_3_exp = n % 3;
    idx_t power_of_10_exp = n / 3;
    
    seq_t power_of_3;
    if (power_of_3_exp == 0) {
        power_of_3 = 1;
    } else if (power_of_3_exp == 1) {
        power_of_3 = 3;
    } else {
        power_of_3 = 9;
    }
    
    seq_t power_of_10 = 1;
    for (idx_t i = 0; i < power_of_10_exp; i++) {
        power_of_10 *= 10;
    }
    
    return power_of_3 * power_of_10;
}
