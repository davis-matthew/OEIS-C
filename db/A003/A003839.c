/*
 * A003839  Order of (usually) simple Chevalley group D_n (7).
 * https://oeis.org/A003839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(n(n-1)) * prod(i=1 to n-1) (7^(2i) - 1) * (7^n - 1) /
 *            gcd(4, 7+(-1)^n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t q = 7;
    seq_t result = 1;
    
    // Compute q^(n*(n-1))
    idx_t exp = n * (n - 1);
    for (idx_t i = 0; i < exp; i++) {
        result *= q;
    }
    
    // Compute prod(i=1 to n-1) (q^(2i) - 1)
    for (idx_t i = 1; i < n; i++) {
        seq_t q_pow = 1;
        for (idx_t j = 0; j < 2 * i; j++) {
            q_pow *= q;
        }
        result *= (q_pow - 1);
    }
    
    // Compute (q^n - 1)
    seq_t q_n = 1;
    for (idx_t j = 0; j < n; j++) {
        q_n *= q;
    }
    result *= (q_n - 1);
    
    // Divide by gcd(4, q + (-1)^n)
    // For q=7: gcd(4, 6) = 2 when n is odd, gcd(4, 8) = 4 when n is even
    seq_t divisor = (n % 2 == 1) ? 2 : 4;
    result /= divisor;
    
    return result;
}
