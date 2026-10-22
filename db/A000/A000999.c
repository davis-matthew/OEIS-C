/*
 * A000999  5-adic valuation of binomial(2*n,n): largest k such that 5^k
 *          divides binomial(2*n, n).
 * https://oeis.org/A000999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k>=1} (floor(2n/5^k) - 2*floor(n/5^k))
 * Range:     u8 n <= 150, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t n_seq = (seq_t)n;
    seq_t two_n = 2 * n_seq;
    seq_t power_of_5 = 5;
    
    while (power_of_5 <= two_n) {
        seq_t term = (two_n / power_of_5) - 2 * (n_seq / power_of_5);
        result += term;
        
        if (power_of_5 > two_n / 5) break;
        power_of_5 *= 5;
    }
    
    return result;
}
