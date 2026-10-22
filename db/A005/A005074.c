/*
 * A005074  Sum of primes = 2 mod 3 dividing n.
 * https://oeis.org/A005074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Additive with a(p^e) = p if p = 2 (mod 3), 0 otherwise
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    
    if (n % 2 == 0) {
        sum += 2;
        while (n % 2 == 0) {
            n /= 2;
        }
    }
    
    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) {
            if (i % 3 == 2) {
                sum += (seq_t)i;
            }
            while (n % i == 0) {
                n /= i;
            }
        }
    }
    
    if (n > 1 && n % 3 == 2) {
        sum += (seq_t)n;
    }
    
    return sum;
}
