/*
 * A007089  Numbers in base 3.
 * https://oeis.org/A007089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 10*a(floor(n/3)) + (n mod 3) for n > 0, a(0) = 0
 * Range:     u8 n <= 26, u16 n <= 242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    
    while (n > 0) {
        result += (seq_t)(n % 3) * power;
        power *= 10;
        n /= 3;
    }
    
    return result;
}
