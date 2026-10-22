/*
 * A003797  Order of simple Chevalley group A_n (7).
 * https://oeis.org/A003797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |PSL(n+1, 7)| = (7^(n(n+1)/2) * ∏(j=1 to n+1) (7^j - 1))
 *            / (6 * gcd(n+1, 6))
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"


seq_t a(idx_t n) {
    seq_t result = 1;
    
    // Compute 7^(n(n+1)/2)
    idx_t exponent = n * (n + 1) / 2;
    result = a000248_pow((seq_t)7, exponent);
    
    // Compute product(j=1 to n+1) (7^j - 1)
    for (idx_t j = 1; j <= n + 1; j++) {
        seq_t power = a000248_pow((seq_t)7, j);
        result *= (power - 1);
    }
    
    // Divide by 6 * gcd(n+1, 6)
    uint64_t g = (uint64_t)(n + 1);
    uint64_t b = 6ULL;
    while (b > 0) {
        uint64_t temp = b;
        b = g % b;
        g = temp;
    }
    
    result /= ((seq_t)6 * (seq_t)g);
    
    return result;
}
