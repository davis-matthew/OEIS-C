/*
 * A134194  a(n) = the smallest positive divisor of n that does not occur
 *          among the exponents in the prime factorization of n.
 * https://oeis.org/A134194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest positive divisor of n that does not occur
 *            among the exponents in the prime factorization of n. For n = 1
 *            the answer is 1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    /* Factor n: primes[] holds the prime bases,
       exps[] holds the corresponding exponents.
       The maximum number of distinct primes for n <= 2^63
       is less than 64, so fixed arrays are safe. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Search the divisors of n in increasing order. */
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            /* Check whether d equals any exponent. */
            bool in_exp = false;
            for (size_t i = 0; i < cnt; ++i) {
                if (d == (idx_t)exps[i]) {   /* exps[i] fits in idx_t */
                    in_exp = true;
                    break;
                }
            }
            if (!in_exp) {
                return (seq_t)d;
            }
        }
    }

    /* The loop should always return, but return n as a safety
       fallback (it is a divisor that cannot be smaller). */
    return (seq_t)n;
}
