/*
 * A076792  Sum_{d divides n} d^2*(-1)^bigomega(d), where bigomega(n) =
 *          A001222(n).
 * https://oeis.org/A076792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, sign, easy
 * Formula:   a(n) = Σ_{d|n} d^2 * (-1)^Ω(d) where Ω(d) is the number of prime
 *            factors of d counted with multiplicity. The function implements
 *            the multiplicative formula: for each prime power p^e, a(p^e) =
 *            (1 + (-1)^e * p^(2e+2)) / (1 + p^2)
 * Range:     i8 n <= 4, i16 n <= 16, i32 n <= 5221, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* External integer types supplied by the build system */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */

    /* Factorize n.  The maximum number of distinct prime factors of a 64‑bit
       integer is less than 64. */
    const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;              /* multiplicative identity */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        size_t e = (size_t)exps[i];

        /* Compute p^(2*e+2) by repeated multiplication.  e is tiny,
           so this loop is inexpensive. */
        idx_t exp = (idx_t)(2 * e + 2);
        seq_t p_pow = 1;
        for (idx_t j = 0; j < exp; ++j) {
            p_pow *= p;
        }

        /* Numerator: 1 + (-1)^e * p^(2e+2) */
        seq_t numerator;
        if ((e & 1) == 0) {         /* e even  -> + sign */
            numerator = p_pow + 1;
        } else {                    /* e odd   -> - sign */
            numerator = 1 - p_pow;
        }

        /* Denominator: 1 + p^2 */
        seq_t denom = 1 + p * p;

        /* Exact division (the formula guarantees divisibility) */
        seq_t term = numerator / denom;

        result *= term;
    }

    return result;
}
