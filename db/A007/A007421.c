/*
 * A007421  Liouville's function: parity of number of primes dividing n (with
 *          multiplicity).
 * https://oeis.org/A007421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2 - (bigomega(n) % 2), where bigomega(n) is the number of
 *            prime factors of n with multiplicity
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t bigomega = 0;
    idx_t temp = n;
    
    // Count factors of 2
    while (temp % 2 == 0) {
        bigomega++;
        temp /= 2;
    }
    
    // Check odd factors from 3 onwards
    for (idx_t i = 3; i * i <= temp; i += 2) {
        while (temp % i == 0) {
            bigomega++;
            temp /= i;
        }
    }
    
    // If temp > 1, then it's a prime factor
    if (temp > 1) {
        bigomega++;
    }
    
    // a(n) = 2 - (bigomega % 2)
    return (seq_t)(2 - (bigomega % 2));
}
