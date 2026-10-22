/*
 * A004665  Powers of 3 written in base 11. (Next term contains a non-decimal
 *          character.)
 * https://oeis.org/A004665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 3^n converted to base 11, then read as decimal
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t power = 1;
    for (idx_t i = 0; i < n; i++) {
        power = power * 3;
    }
    
    seq_t result = 0;
    seq_t base10_place = 1;
    while (power > 0) {
        seq_t digit = power % 11;
        result = result + digit * base10_place;
        base10_place = base10_place * 10;
        power = power / 11;
    }
    
    return result;
}
