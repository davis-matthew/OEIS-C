/*
 * A005070  Sum of primes == 1 (mod 3) dividing n.
 * https://oeis.org/A005070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime divisors p of n where p ≡ 1 (mod 3)
 * Range:     u8 n <= 270, u16 n <= 9392, u32 n <= 9392, u64 n <= 9392,
 *            u128 n <= 9392, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    // Find the smallest prime factor
    idx_t p = 0;
    if (n % 2 == 0) {
        p = 2;
    } else {
        for (idx_t i = 3; i * i <= n; i += 2) {
            if (n % i == 0) {
                p = i;
                break;
            }
        }
        if (p == 0) {
            p = n;  // n is prime
        }
    }
    
    // Remove all factors of p from n
    idx_t quotient = n;
    while (quotient % p == 0) {
        quotient /= p;
    }
    
    // Recursively compute a(quotient)
    seq_t result = a(quotient);
    
    // Add p if p ≡ 1 (mod 3)
    if (p % 3 == 1) {
        result += (seq_t)p;
    }
    
    return result;
}
