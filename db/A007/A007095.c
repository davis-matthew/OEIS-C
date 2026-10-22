/*
 * A007095  Numbers in base 9.
 * https://oeis.org/A007095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = interpret base-9 digits of n as base-10 (n-th number
 *            without digit 9)
 * Range:     u8 n <= 212, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    
    idx_t temp = n;
    while (temp > 0) {
        seq_t digit = (seq_t)(temp % 9);
        result += digit * power;
        power *= 10;
        temp /= 9;
    }
    
    return result;
}
