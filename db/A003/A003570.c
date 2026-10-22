/*
 * A003570  For n>0, a(n) = least positive number m such that 8^m == +1 or -1
 *          (mod 2n + 1), with a(0) = 0 by convention.
 * https://oeis.org/A003570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive m such that 8^m ≡ ±1 (mod 2n+1)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    uint64_t mod = 2 * n + 1;
    uint64_t power = 8 % mod;
    
    for (uint64_t m = 1; ; m++) {
        if (power == 1 || power == mod - 1) {
            return (seq_t)m;
        }
        power = ((__uint128_t)power * 8) % mod;
    }
}
