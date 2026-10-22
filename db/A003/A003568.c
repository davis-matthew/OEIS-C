/*
 * A003568  Least number m such that 12^m == +- 1 (mod 12n + 1).
 * https://oeis.org/A003568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that 12^m ≡ ±1 (mod 12n+1)
 * Range:     u8 n <= 21, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t mod = (seq_t)n * 12 + 1;
    seq_t power = 12 % mod;
    seq_t minus_one = mod - 1;
    
    for (seq_t m = 1; m <= mod; m++) {
        if (power == 1 || power == minus_one) {
            return (seq_t)m;
        }
        power = (power * 12) % mod;
    }
    
    return (seq_t)mod;
}
