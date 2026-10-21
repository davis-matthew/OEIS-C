/*
 * A096960  a(n) = Sum_{0<d|n, n/d odd} d^5.
 * https://oeis.org/A096960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{0<d|n, n/d odd} d^5
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7127,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) can be computed multiplicatively:
   For n = 2^e * m (m odd), a(n) = 2^(5e) * \prod_{p^k||m} (1 + p^5 + p^10 + … + p^{5k})  */

#include <stdint.h>
#include "../utils/seq_pow_seq.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, just safety */
    uint64_t temp = (uint64_t)n;
    seq_t result = 1;

    /* factor the power of 2 separately */
    idx_t e2 = 0;
    while ((temp & 1ULL) == 0) {
        e2++;
        temp >>= 1;
    }
    if (e2 > 0) {
        /* 2^(5*e2) = (2^5)^e2 = 32^e2 */
        seq_t contrib = 1;
        for (idx_t i = 0; i < e2; ++i) contrib *= (seq_t)32;
        result *= contrib;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }
            /* compute sum 1 + p^5 + p^10 + ... + p^(5e) */
            seq_t p5 = 1;
            for (idx_t i = 0; i < 5; ++i) p5 *= (seq_t)p;
            seq_t sum = 1;
            seq_t term = p5;
            for (idx_t i = 1; i <= e; ++i) {
                sum += term;
                term *= p5;
            }
            result *= sum;
        }
    }

    if (temp > 1) { /* remaining odd prime factor (exponent 1) */
        seq_t p5 = 1;
        for (idx_t i = 0; i < 5; ++i) p5 *= (seq_t)temp;
        seq_t sum = 1 + p5;
        result *= sum;
    }

    return result;
}
