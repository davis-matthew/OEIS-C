/*
 * A003567  Least number m such that 11^m == +- 1 (mod 11n + 1).
 * https://oeis.org/A003567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that 11^m == ±1 (mod 11n+1)
 * Range:     u8 n <= 61, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    uint64_t modulus = (uint64_t)n * 11 + 1;
    uint64_t power = 11;
    
    for (uint64_t m = 1; m < modulus; m++) {
        if (power == 1) return (seq_t)m;
        if (power == modulus - 1) return (seq_t)m;
        power = (power * 11) % modulus;
    }
    
    return (seq_t)modulus;
}
