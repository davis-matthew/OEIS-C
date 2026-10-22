/*
 * A003569  For n>0, a(n) = least positive number m such that 4^m == +1 or -1
 *          (mod 2n + 1), with a(0) = 0 by convention.
 * https://oeis.org/A003569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   For n>0, a(n) = least positive m such that 4^m ≡ ±1 (mod 2n+1);
 *            a(0) = 0
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t mod = (seq_t)n * 2 + 1;
    seq_t power = 4 % mod;
    
    for (seq_t m = 1; m <= mod; m++) {
        if (power == 1 || power == mod - 1) {
            return m;
        }
        power = (power * 4) % mod;
    }
    
    return 0;
}
