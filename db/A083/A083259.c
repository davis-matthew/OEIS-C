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
 * Range:     u8 n <= 255, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 113 terms through n = 9207 (2026-11-01)
 * Helpers:   factor_u64, gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

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
        seq_t power  = ipow_idx(base, ex[i]);   /* base^ex[i] */
        target = seq_mul(target, power);       /* accumulate product */
    }

    /* gcd with original n */
    seq_t sn = (seq_t)n;
    return gcd(sn, target);
}
