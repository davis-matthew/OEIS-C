/*
 * A005067  Sum of cubes of odd primes dividing n.
 * https://oeis.org/A005067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of cubes of odd primes dividing n. Additive with
 *            a(p^e) = 0 if p = 2, p^3 otherwise
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t result = 0;
    idx_t m = n;
    
    // Remove all factors of 2
    while (m % 2 == 0) {
        m /= 2;
    }
    
    // For each odd prime factor, add its cube
    for (idx_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) {
            seq_t prime_cubed = (seq_t)i * (seq_t)i * (seq_t)i;
            result += prime_cubed;
            while (m % i == 0) {
                m /= i;
            }
        }
    }
    
    // If m is still > 1, then it's a prime factor
    if (m > 1) {
        seq_t prime_cubed = (seq_t)m * (seq_t)m * (seq_t)m;
        result += prime_cubed;
    }
    
    return result;
}
