/*
 * A001227  Number of odd divisors of n.
 * https://oeis.org/A001227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult, core
 * Formula:   a(n) = number of odd divisors of n = τ(odd_part(n))
 * Range:     u8 n <= 1631, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    
    // Remove all factors of 2 to get the odd part
    while (m % 2 == 0) {
        m /= 2;
    }
    
    // Count divisors of m using trial division
    seq_t divisor_count = 1;
    
    // Check odd divisors starting from 3
    for (seq_t d = 3; d * d <= m; d += 2) {
        seq_t exp = 0;
        while (m % d == 0) {
            exp++;
            m /= d;
        }
        if (exp > 0) {
            divisor_count *= (exp + 1);
        }
    }
    
    // If m > 1, it's a prime factor with exponent 1
    if (m > 1) {
        divisor_count *= 2;
    }
    
    return divisor_count;
}
