/*
 * A005087  Number of distinct odd primes dividing n.
 * https://oeis.org/A005087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct odd primes dividing n (additive:
 *            a(p^e) = 0 if p = 2, else 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t count = 0;
    idx_t temp = n;
    
    // Skip factor 2
    while (temp % 2 == 0) {
        temp /= 2;
    }
    
    // Count odd prime factors
    for (idx_t i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            count++;
            while (temp % i == 0) {
                temp /= i;
            }
        }
    }
    
    // If temp > 1, it's a prime factor
    if (temp > 1) {
        count++;
    }
    
    return count;
}
