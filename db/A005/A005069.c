/*
 * A005069  Sum of odd primes dividing n.
 * https://oeis.org/A005069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sum of distinct odd primes dividing n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Remove factors of 2
    idx_t odd_n = n;
    while (odd_n % 2 == 0) {
        odd_n /= 2;
    }
    
    seq_t sum = 0;
    
    // Find odd prime factors
    for (idx_t p = 3; p <= odd_n / p; p += 2) {
        if (odd_n % p == 0) {
            sum += (seq_t)p;
            // Remove all factors of p
            while (odd_n % p == 0) {
                odd_n /= p;
            }
        }
    }
    
    // If odd_n > 1, then it's a prime factor
    if (odd_n > 1) {
        sum += (seq_t)odd_n;
    }
    
    return sum;
}
