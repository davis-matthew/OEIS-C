/*
 * A004668  Powers of 3 written in base 26. (Next term contains a non-decimal
 *          digit.)
 * https://oeis.org/A004668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   3^n converted to base 26, interpreted as decimal
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow3(idx_t n) {
    seq_t result = 1;
    seq_t base = 3;
    while (n > 0) {
        if (n % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        n = n / 2;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t power_of_3 = pow3(n);
    
    seq_t result = 0;
    seq_t base = 1;
    
    while (power_of_3 > 0) {
        seq_t digit = power_of_3 % 26;
        result = result + digit * base;
        base = base * 10;
        power_of_3 = power_of_3 / 26;
    }
    
    return result;
}
