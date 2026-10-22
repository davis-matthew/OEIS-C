/*
 * A001732  8 in base 10-n.
 * https://oeis.org/A001732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = representation of 8 in base (10-n), read as base 10
 *            number
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n > 9) {
        return (seq_t)0;  /* Sequence is finite, defined only for 0 <= n <= 9 */
    }
    
    idx_t base = 10 - n;
    
    /* For unary (base 1), 8 is represented as eight 1's: 11111111 */
    if (base == 1) {
        return (seq_t)11111111;
    }
    
    /* Convert 8 to the given base, read the digits as a base-10 number */
    seq_t result = 0;
    idx_t value = 8;
    seq_t multiplier = 1;
    
    while (value > 0) {
        idx_t digit = value % base;
        result = result + (seq_t)digit * multiplier;
        multiplier = multiplier * 10;
        value = value / base;
    }
    
    return result;
}
