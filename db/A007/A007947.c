/*
 * A007947  Largest squarefree number dividing n: the squarefree kernel of n,
 *          rad(n), radical of n.
 * https://oeis.org/A007947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = Product of distinct prime factors of n (squarefree
 *            kernel)
 * Range:     u8 n <= 78, u16 n <= 9799, u32 n <= 9799, u64 n <= 9799,
 *            u128 n <= 9799, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t m = (seq_t)n;
    
    // Check for factor 2
    if (m % 2 == 0) {
        result *= 2;
        while (m % 2 == 0) {
            m /= 2;
        }
    }
    
    // Check for odd factors from 3 onwards
    for (seq_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            result *= p;
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    
    // If m > 1, then it's a prime factor
    if (m > 1) {
        result *= m;
    }
    
    return result;
}
