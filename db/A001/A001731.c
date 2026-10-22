/*
 * A001731  a(n) is 9 written in base 10-n.
 * https://oeis.org/A001731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) is 9 written in base 10-n
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n >= 10) {
        return 0;
    }
    
    idx_t base = 10 - n;
    
    if (base == 1) {
        return 111111111;
    }
    
    seq_t result = 0;
    seq_t value = 9;
    seq_t multiplier = 1;
    
    while (value > 0) {
        seq_t digit = value % (seq_t)base;
        result += digit * multiplier;
        multiplier *= 10;
        value /= (seq_t)base;
    }
    
    return result;
}
