/*
 * A063919  Sum of proper unitary divisors (or unitary aliquot parts) of n,
 *          including 1.
 * https://oeis.org/A063919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = sum of proper unitary divisors of n (excluding n itself,
 *            including 1). For n = 1 return 1. A unitary divisor is a divisor
 *            d such that gcd(d, n/d) = 1. For n > 1 with factorisation n = ∏
 *            p_i^e_i, the sum of all unitary divisors is ∏ (1 + p_i^e_i).
 *            Hence a(n) = ∏ (1 + p_i^e_i) – n
 * Range:     u8 n <= 209, u16 n <= 9570, u32 n <= 9570, u64 n <= 9570,
 *            u128 n <= 9570, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    seq_t prod = (seq_t)1;
    uint64_t p = 2;

    while (p <= temp / p) {
        if (temp % p == 0) {
            uint64_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
            }
            prod *= (seq_t)(1) + (seq_t)power;
        }
        p += (p == 2 ? 1 : 2);
    }
    if (temp > 1)
        prod *= (seq_t)(1) + (seq_t)temp;

    return prod - (seq_t)n;
}
