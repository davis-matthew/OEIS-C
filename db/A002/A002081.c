/*
 * A002081  Numbers congruent to {2, 4, 8, 16} (mod 20).
 * https://oeis.org/A002081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 5*n + (1/2)*(3 + (-1)^n)*(-1)^(n(n+1)/2)
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = (seq_t)n * 5;
    
    switch (n & 3) {
    case 0: return base + 2;
    case 1: return base - 1;
    case 2: return base - 2;
    case 3: return base + 1;
    }
    
    return base;
}
