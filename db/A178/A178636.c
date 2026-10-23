/*
 * A178636  If n = Product (p_i^k_i) for i = 1, ..., j then a(n) is the sum of
 *          the divisors d that are not in the set {1, p_1^k_1, p_2^k_2, ...,
 *          p_j^k_j}.
 * https://oeis.org/A178636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - (1 + Σ p_i^k_i) where sigma(n) is the sum of
 *            all divisors of n and the sum runs over the distinct prime
 *            powers p_i^k_i dividing n. This equals A178636's definition
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t temp = n;
    seq_t sigma = 1;
    seq_t sumPrimePowers = 0;

    /* handle factor 2 */
    if (temp % 2 == 0) {
        seq_t p_pow = 1;
        seq_t term = 1;          /* σ part: 1 + 2 + 2^2 + ... */
        while (temp % 2 == 0) {
            temp /= 2;
            p_pow *= (seq_t)2;
            term += p_pow;
        }
        sigma *= term;
        sumPrimePowers += p_pow; /* 2^k */
    }

    /* handle odd prime factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            seq_t p_pow = 1;
            seq_t term = 1;
            while (temp % p == 0) {
                temp /= p;
                p_pow *= (seq_t)p;
                term += p_pow;
            }
            sigma *= term;
            sumPrimePowers += p_pow;
        }
    }

    /* remaining prime if any */
    if (temp > 1) {
        seq_t p_pow = temp;     /* last prime power is temp^1 */
        seq_t term = 1 + p_pow; /* σ part for exponent 1 */
        sigma *= term;
        sumPrimePowers += p_pow;
    }

    seq_t result = sigma;
    result -= 1;                /* remove divisor 1 */
    result -= sumPrimePowers;   /* remove prime powers */
    return result;
}
