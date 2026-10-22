/*
 * A006364  Numbers k with an even number of 1's in binary, ignoring last bit.
 * https://oeis.org/A006364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, nice, easy
 * Formula:   a(n) = 2*n + (-1)^(n-A000120(n-1)) - (3+(-1)^n)/2
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    uint64_t pc = __builtin_popcountll((uint64_t)(n - 1));
    seq_t result = (seq_t)2 * (seq_t)n;
    
    if (((n - pc) & 1) == 0) {
        result += 1;
    } else {
        result -= 1;
    }
    
    if ((n & 1) == 0) {
        result -= 2;
    } else {
        result -= 1;
    }
    
    return result;
}
