/*
 * A007964  Numbers k such that product of proper divisors of k is <= k; i.e.,
 *          product of divisors of k is <= k^2.
 * https://oeis.org/A007964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers k such that product of divisors of k is <= k^2,
 *            equivalently numbers with at most 4 divisors. Computed by
 *            iterating through k and checking the divisor count
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int count_divisors(seq_t k) {
    if (k == 1) return 1;
    
    int count = 1;
    seq_t temp = k;
    
    // Check for factor 2
    int exp = 0;
    while (temp % 2 == 0) {
        exp++;
        temp /= 2;
    }
    if (exp > 0) count *= (exp + 1);
    
    // Check for odd factors
    for (seq_t i = 3; i <= temp / i; i += 2) {
        exp = 0;
        while (temp % i == 0) {
            exp++;
            temp /= i;
        }
        if (exp > 0) count *= (exp + 1);
    }
    
    // If temp > 1, then it's a prime factor
    if (temp > 1) count *= 2;
    
    return count;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;
    
    while (count < n) {
        k++;
        if (count_divisors(k) <= 4) {
            count++;
        }
    }
    
    return k;
}
