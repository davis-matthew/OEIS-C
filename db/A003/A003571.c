/*
 * A003571  Order of 3 mod 3n+1.
 * https://oeis.org/A003571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 3 modulo (3n+1)
 * Range:     u8 n <= 68, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = 3 * (seq_t)n + 1;
    
    if (m == 1) {
        return 1;
    }
    
    seq_t power = 3 % m;
    seq_t order = 1;
    
    while (power != 1) {
        power = (power * 3) % m;
        order++;
    }
    
    return order;
}
