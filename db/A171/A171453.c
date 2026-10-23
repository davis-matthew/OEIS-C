/*
 * A171453  a(n) = sum_i p_i^(e_i-1) where n = product_i p_i^e_i is the prime
 *          number decomposition of n.
 * https://oeis.org/A171453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{p^e‖n} p^(e-1) (sum of prime factors raised to the
 *            (exponent − 1) power)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t sum = 0;
    uint64_t m = (uint64_t)n;
    while (m > 1) {
        uint64_t p = _smallest_prime_factor(m);
        int e = 0;
        while (m % p == 0) {
            m /= p;
            e++;
        }
        seq_t base = (seq_t)p;
        seq_t pow = 1;
        for (idx_t i = 0; i < (idx_t)(e - 1); ++i) pow *= base;
        sum += pow;
    }
    return sum;
}
