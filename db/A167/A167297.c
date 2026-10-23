/*
 * A167297  Totally multiplicative sequence with a(p) = 5*(p-2) for prime p.
 * https://oeis.org/A167297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with a(p^e) = (5*(p-2))^e for prime p,
 *            a(1) = 1, and a(n) = 0 for all even n ≥ 2
 * Range:     u8 n <= 34, u16 n <= 874, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Simple exponentiation by squaring for seq_t values. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1) {
            res = res * base;
        }
        base = base * base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Handle the first term. */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Even numbers > 1 are zero. */
    if ((n & 1ULL) == 0) {
        return (seq_t)0;
    }

    /* Factor n into primes. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t e = exps[i];
        /* Compute base = 5*(p-2). */
        seq_t base = (seq_t)(5 * (p - 2));
        result *= pow_seq_t(base, e);
    }
    return result;
}
