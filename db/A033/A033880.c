/*
 * A033880  Abundance of n, or (sum of divisors of n) - 2n.
 * https://oeis.org/A033880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, nice
 * Formula:   a(n) = A000203(n) - A005843(n) = sigma(n) - 2*n where sigma(n)
 *            is the sum of all positive divisors of n
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* factor the integer n */
    const size_t MAX_FACTORS = 64;          /* more than enough for n <= UINT64_MAX */
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute sigma(n) = product over p^e of (p^(e+1)-1)/(p-1) */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t term = (seq_t)1;
        seq_t pow = (seq_t)1;
        for (uint64_t e = 0; e < exps[i]; ++e) {
            pow *= p;
            term += pow;
        }
        sigma *= term;
    }

    seq_t two_n = (seq_t)n + (seq_t)n;
    return sigma - two_n;
}
