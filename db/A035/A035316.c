/*
 * A035316  Sum of the square divisors of n.
 * https://oeis.org/A035316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum_{k^2 | n} k^2 (sum of the square divisors of n)
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

/* Helper: computes the contribution of a prime power p^e to a(n). */
static seq_t _term_prime_pow(uint64_t p, idx_t e)
{
    seq_t pt = (seq_t)p;
    /* denominator p^2 - 1 */
    seq_t denom = seq_pow(pt, (idx_t)2) - 1;
    seq_t num;
    if ((e & 1) == 0) {                /* even exponent: e = 2m */
        num = seq_pow(pt, e + 2) - 1;  /* sum_{j=0..m} p^{2j} = (p^{e+2}-1)/(p^2-1) */
    } else {                           /* odd exponent: e = 2m+1 */
        num = seq_pow(pt, e + 1) - 1;  /* sum_{j=0..m} p^{2j} = (p^{e+1}-1)/(p^2-1) */
    }
    return num / denom;                /* exact division */
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                     /* a(1) = 1, also a(0) not used */

    uint64_t nn = (uint64_t)n;

    /* factor nn; arrays large enough for any 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t     e = (idx_t)exps[i];
        result *= _term_prime_pow(p, e);
    }
    return result;
}
