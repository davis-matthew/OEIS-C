/*
 * A001733  7 in base 10-n.
 * https://oeis.org/A001733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   7 written in base (10-n), interpreted as a decimal number
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n >= 10) {
        return 0;
    }
    
    int base = 10 - (int)n;
    
    if (base == 1) {
        return (seq_t)1111111;
    }
    
    seq_t num = 7;
    seq_t result = 0;
    seq_t power = 1;
    
    while (num > 0) {
        seq_t digit = num % (seq_t)base;
        result += digit * power;
        power *= (seq_t)10;
        num /= (seq_t)base;
    }
    
    return result;
}
