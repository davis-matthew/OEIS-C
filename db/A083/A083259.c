/*
 * A083259  a(n) = gcd(n, A071364(n)), where A071364(n) is the smallest number
 *          with same sequence of exponents in canonical prime factorization
 *          as n.
 * https://oeis.org/A083259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A071364(n)) where A071364(n) is the smallest
 *            integer whose canonical prime factorisation has the same
 *            sequence of exponents as n
 * Range:     u8 n <= 255, u16 n <= 9207, u32 n <= 9207, u64 n <= 9207,
 *            u128 n <= 9207, big any n
 * Helpers:   factor_u64, gcd, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/factor_u64.h"

static const uint64_t SMALL_PRIMES[20] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL,
    59ULL, 61ULL, 67ULL, 71ULL
};

seq_t a(idx_t n)
{
    /* factor n into primes and exponents */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* compute A071364(n) = ∏ SMALL_PRIMES[i]^ex[i] */
    seq_t target = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base   = (seq_t)SMALL_PRIMES[i];
        seq_t power  = seq_pow(base, ex[i]);   /* base^ex[i] */
        target = seq_mul(target, power);       /* accumulate product */
    }

    /* gcd with original n */
    seq_t sn = (seq_t)n;
    return gcd(sn, target);
}
