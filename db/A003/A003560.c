/*
 * A003560  Least number m such that 4^m == +- 1 (mod 4n + 1).
 * https://oeis.org/A003560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that 4^m ≡ ±1 (mod 4n+1)
 * Range:     u8 n <= 63, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t modulus = (seq_t)n * 4 + 1;
    
    seq_t power = 4 % modulus;
    for (seq_t m = 1; ; m++) {
        if (power == 1 || power == modulus - 1) {
            return m;
        }
        power = (power * 4) % modulus;
    }
}
