/*
 * A007041  State assignments for n-state machine.
 * https://oeis.org/A007041
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^s)! / ((2^s - n)! * s!) where s = ceiling(log_2 n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 9, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Find s = ceiling(log_2(n))
    idx_t s = 0;
    idx_t power = 1;
    while (power < n) {
        power *= 2;
        s++;
    }
    
    idx_t m = power; // m = 2^s
    
    // Compute the falling factorial P(m, n) = m * (m-1) * ... * (m-n+1)
    seq_t result = 1;
    for (idx_t i = 0; i < n; i++) {
        result *= (seq_t)(m - i);
    }
    
    // Divide by s!
    for (idx_t i = 1; i <= s; i++) {
        result /= (seq_t)i;
    }
    
    return result;
}
