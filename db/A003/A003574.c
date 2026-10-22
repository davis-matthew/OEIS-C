/*
 * A003574  Order of 4 mod 4n-1.
 * https://oeis.org/A003574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = order of 4 modulo (4n-1)
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n * 4 - 1;
    seq_t result = 1;
    seq_t power = 4 % m;
    
    while (power != 1) {
        power = (power * 4) % m;
        result++;
    }
    
    return result;
}
