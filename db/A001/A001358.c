/*
 * A001358  Semiprimes (or biprimes): products of two primes.
 * https://oeis.org/A001358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   a(n) = n-th semiprime, where semiprimes are numbers with exactly
 *            2 prime factors (counting multiplicity)
 * Range:     u8 n <= 82, u16 n <= 15658, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 77 terms through n = 8380 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int count_prime_factors(seq_t num) {
    int count = 0;
    seq_t temp = num;
    
    // Count factors of 2
    while (temp % 2 == 0) {
        count++;
        temp /= 2;
        if (count > 2) return count;
    }
    
    // Count odd factors
    for (seq_t d = 3; d * d <= temp; d += 2) {
        while (temp % d == 0) {
            count++;
            temp /= d;
            if (count > 2) return count;
        }
    }
    
    // If temp > 1, it's a prime factor
    if (temp > 1) count++;
    
    return count;
}

seq_t a(idx_t n) {
    seq_t found = 0;
    
    for (seq_t candidate = 2; ; candidate++) {
        if (count_prime_factors(candidate) == 2) {
            found++;
            if (found == (seq_t)n) return candidate;
        }
    }
}
