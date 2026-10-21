/*
 * A055230  Greatest common divisor of largest square dividing n! and
 *          squarefree part of n!.
 * https://oeis.org/A055230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all primes p ≤ n such that the exponent of p
 *            in n! is odd and at least 3. Equivalently, a(n) = gcd(largest
 *            square dividing n!, squarefree part of n!)
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 122, u64 n <= 216,
 *            u128 n <= 484, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;            /* undefined for n < 1, return 1 */

    if (n < 2)                       /* a(1) = 1 */
        return (seq_t)1;

    /* ---------- Sieve of Eratosthenes to list primes ≤ n ---------- */
    idx_t size = n + 1;
    bool *is_comp = (bool *)calloc((size_t)size, sizeof(bool));
    uint64_t *primes = (uint64_t *)calloc((size_t)n, sizeof(uint64_t));
    size_t prime_cnt = 0;

    for (idx_t p = 2; p <= n; ++p) {
        if (!is_comp[p]) {
            primes[prime_cnt++] = (uint64_t)p;
            if (p * p <= n) {
                for (idx_t j = p * p; j <= n; j += p)
                    is_comp[j] = true;
            }
        }
    }

    /* ---------- Compute the product of qualifying primes ---------- */
    seq_t result = (seq_t)1;

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint64_t p = primes[idx];
        idx_t e = 0;
        idx_t power = p;
        while (power <= n) {
            e += n / power;
            if (power > n / p)   /* avoid overflow on next multiplication */
                break;
            power *= p;
        }
        if ((e & 1) && e >= 3)   /* odd exponent and at least 3 */
            result = seq_mul(result, (seq_t)p);
    }

    free(is_comp);
    free(primes);
    return result;
}
