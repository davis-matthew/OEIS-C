/*
 * A001218  a(n) = 3^n mod 100.
 * https://oeis.org/A001218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 100, period 20
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    n = n % 20;
    
    uint64_t result = 1;
    uint64_t base = 3;
    
    while (n > 0) {
        if (n & 1) {
            result = (result * base) % 100;
        }
        base = (base * base) % 100;
        n >>= 1;
    }
    
    return (seq_t)result;
}
