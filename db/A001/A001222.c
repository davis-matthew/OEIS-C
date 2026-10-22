/*
 * A001222  Number of prime divisors of n counted with multiplicity (also
 *          called big omega of n, bigomega(n) or Omega(n)).
 * https://oeis.org/A001222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, core, changed
 * Formula:   a(n) = Sum of exponents in prime factorization of n
 * Range:     u8 n <= 9751, u16 n <= 9751, u32 n <= 9751, u64 n <= 9751,
 *            u128 n <= 9751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t count = 0;
    idx_t temp = n;
    
    // Count factors of 2
    while (temp % 2 == 0) {
        count++;
        temp /= 2;
    }
    
    // Count odd factors from 3 onwards
    for (idx_t i = 3; i <= temp / i; i += 2) {
        while (temp % i == 0) {
            count++;
            temp /= i;
        }
    }
    
    // If temp > 1, then it's a prime factor
    if (temp > 1) {
        count++;
    }
    
    return count;
}
