/*
 * A002751  Sum of logarithmic numbers.
 * https://oeis.org/A002751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n+1} A000005(m)*binomial(n, m-1)*(m-1)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   count_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_divisors.h"

/* Compute binomial coefficient C(n, k) */
static seq_t binomial(idx_t n, idx_t k) {
    if (k > n || k < 0) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    
    seq_t result = 1;
    for (idx_t i = 1; i <= k; i++) {
        result = result * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t factorial = 1;
    
    for (idx_t m = 1; m <= n + 1; m++) {
        idx_t divisors = count_divisors(m);
        seq_t binom = binomial(n, m - 1);
        
        result += (seq_t)divisors * binom * factorial;
        
        if (m < n + 1) {
            factorial *= (seq_t)m;
        }
    }
    
    return result;
}
