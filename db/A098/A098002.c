/*
 * A098002  Sum of squares of distinct prime divisors p of n, where each p <=
 *          sqrt(n).
 * https://oeis.org/A098002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098002: sum of squares of distinct prime divisors p of n, with p <= sqrt(n) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt) */

seq_t a(idx_t n)
{
    /* For n < 2 the sum is zero */
    if (n < 2) return (seq_t)0;

    /* Temporary storage for prime factorisation */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    seq_t sqn = (seq_t)n;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t p2 = p * p;
        /* Include p only if p <= sqrt(n) i.e. p*p <= n */
        if (p2 <= sqn) {
            sum += p2;
        }
    }
    return sum;
}
