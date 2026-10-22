/*
 * A005345  Number of elements of a free idempotent monoid on n letters.
 * https://oeis.org/A005345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (C(n, k) * Prod_{i=1..k} (k-i+1)^(2^i))
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a000248_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t k = 0; k <= n; k++) {
        seq_t binom_nk = binom(n, k);
        
        seq_t product = 1;
        idx_t power_of_2 = 2;
        for (idx_t i = 1; i <= k; i++) {
            seq_t base = (seq_t)(k - i + 1);
            product *= a000248_pow(base, power_of_2);
            power_of_2 *= 2;
        }
        
        result += binom_nk * product;
    }
    
    return result;
}
