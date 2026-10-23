/*
 * A179312  Largest semiprime dividing n, or 0 if no semiprime divides n.
 * https://oeis.org/A179312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max{ k ∈ A001358 | k divides n }, 0 if no such k
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* we use the supplied factorisation routine */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;

    /* factor m: primes and their exponents */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    uint64_t best = 0;

    /* largest product of two distinct primes dividing n */
    for (size_t i = 0; i < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j) {
            uint64_t cand = pr[i] * pr[j];
            if (cand > best)
                best = cand;
        }

    /* largest square of a prime dividing n (exponent >= 2) */
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] >= 2) {
            uint64_t cand = pr[i] * pr[i];
            if (cand > best)
                best = cand;
        }

    return (seq_t)best;
}
