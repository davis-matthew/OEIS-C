/*
 * A000371  a(n) = Sum_{k=0..n} (-1)^(n-k)*binomial(n,k)*2^(2^k).
 * https://oeis.org/A000371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k)*binomial(n,k)*2^(2^k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   binomial, power_of_2_to_power_of_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/power_of_2_to_power_of_2.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 0; k <= n; k++) {
        seq_t binom = binomial(n, k);
        seq_t power = power_of_2_to_power_of_2(k);
        seq_t term = binom * power;
        
        if ((n - k) & 1) {
            result -= term;
        } else {
            result += term;
        }
    }
    return result;
}
