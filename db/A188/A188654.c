/*
 * A188654  Numbers k such that the maximum exponent in its prime
 *          factorization does not equal the number of positive exponents
 *          (A051903(k) <> A001221(k)).
 * https://oeis.org/A188654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by scanning successive integers and selecting those
   whose maximum exponent in their prime factorization is not equal
   to the number of distinct prime divisors. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)     /* undefined for index 0, return 0 */
        return (seq_t)0;

    idx_t count = 0;
    uint64_t k = 2;           /* start from the smallest composite */

    while (count < n) {
        uint64_t primes[20];
        uint64_t exps[20];
        size_t  cnt = 0;

        factor_u64(k, primes, exps, &cnt);

        /* determine the largest exponent */
        uint64_t maxexp = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (exps[i] > maxexp)
                maxexp = exps[i];
        }

        /* include k if maxexp != number of distinct primes */
        if (maxexp != (uint64_t)cnt) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;     /* next integer to test */
    }

    /* should never reach here */
    return (seq_t)0;
}
