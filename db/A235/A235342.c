/*
 * A235342  Sum of exponents in the (unique) factorization of n as a ratio of
 *          p! terms, p prime.
 * https://oeis.org/A235342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the additive homomorphism from Q⁺ to Z given by a(p!) =
 *            1 for each prime p. It is computed by determining a(p) for
 *            primes via the equations a(p) = 1 - Σ_{q<p} e(p,q)·a(q), where
 *            e(p,q) is the exponent of q in p!. For an integer n, a(n) =
 *            Σ_{p^e‖n} e·a(p) (multiplicativity)
 * Range:     i8 n <= 5040, i16 n <= 5040, i32 n <= 5040, i64 n <= 5040,
 *            i128 n <= 5040, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static seq_t exp_in_factorial(uint64_t num, uint64_t prime) {
    seq_t exp = 0;
    uint64_t d = prime;
    while (d <= num) {
        exp += (seq_t)(num / d);
        if (d > num / prime) break;       /* prevent overflow */
        d *= prime;
    }
    return exp;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;          /* a(1) = 0 by convention */

    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(n, &primes, &primes_cnt);

    /* Allocate array for a(p) values */
    seq_t *a_val = (seq_t *)malloc(sizeof(seq_t) * primes_cnt);
    if (!a_val) {
        free(primes);
        return (seq_t)0;                /* fallback, memory error */
    }

    /* Compute a(p) for each prime in ascending order */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t sum = 0;
        for (size_t j = 0; j < i; ++j) {
            uint64_t q = primes[j];
            seq_t e = exp_in_factorial(p, q);
            sum += e * a_val[j];
        }
        a_val[i] = (seq_t)1 - sum;       /* a(p) = 1 - sum */
    }

    /* Factor n and accumulate a(n) = Σ e·a(p) */
    seq_t result = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > temp / p) break;   /* p*p > temp */

        if (temp % p == 0) {
            unsigned exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            result += (seq_t)exp * a_val[i];
        }
    }

    /* If a prime factor remains (temp > 1) */
    if (temp > 1) {
        for (size_t i = 0; i < primes_cnt; ++i)
            if (primes[i] == temp) {
                result += a_val[i];
                break;
            }
    }

    free(a_val);
    free(primes);
    return result;
}
