/*
 * A001157  a(n) = sigma_2(n): sum of squares of divisors of n.
 * https://oeis.org/A001157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, core, nice, easy, mult
 * Formula:   a(n) = sigma_2(n), sum of squares of divisors; multiplicative
 *            with a(p^e) = 1 + p^2 + p^4 + ... + p^(2e)
 * Range:     u8 n <= 14, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    
    for (idx_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) {
                e++;
                n /= p;
            }
            seq_t p_val = (seq_t)p;
            seq_t p_sq = p_val * p_val;
            seq_t term = 0;
            seq_t p_power = 1;
            for (idx_t i = 0; i <= e; i++) {
                term += p_power;
                p_power *= p_sq;
            }
            result *= term;
        }
    }
    
    if (n > 1) {
        seq_t n_val = (seq_t)n;
        seq_t n_sq = n_val * n_val;
        result *= (1 + n_sq);
    }
    
    return result;
}
