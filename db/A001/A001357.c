/*
 * A001357  Powers of 2 written in base 9.
 * https://oeis.org/A001357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = 2^n written in base 9, interpreted as a base-10 number
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < n; i++) {
        power *= (seq_t)2;
    }
    
    seq_t result = 0;
    seq_t multiplier = 1;
    while (power > 0) {
        seq_t digit = power % 9;
        result = result + digit * multiplier;
        multiplier *= 10;
        power /= 9;
    }
    
    return result;
}
