/*
 * A003561  Least number m such that 5^m == +- 1 (mod 5n + 1).
 * https://oeis.org/A003561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that 5^m == +- 1 (mod 5n + 1)
 * Range:     u8 n <= 50, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t mod = (seq_t)n * 5 + 1;
    seq_t power = 5 % mod;
    
    for (seq_t m = 1; m <= mod; ++m) {
        if (power == 1 || power == mod - 1) {
            return m;
        }
        power = (power * 5) % mod;
    }
    
    return mod;
}
