/*
 * A001735  5 in base 10-n.
 * https://oeis.org/A001735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) is 5 converted to base (10-n) and returned as a decimal
 *            number
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n >= 10) {
        return 0;
    }
    
    seq_t base = (seq_t)10 - (seq_t)n;
    
    if (base == 1) {
        return 11111;
    }
    
    seq_t value = (seq_t)5;
    seq_t result = 0;
    seq_t power = 1;
    
    while (value > 0) {
        seq_t digit = value % base;
        result = result + digit * power;
        power = power * 10;
        value = value / base;
    }
    
    return result;
}
