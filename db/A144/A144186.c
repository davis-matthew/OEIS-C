/*
 * A144186  Numerators of series expansion of the e.g.f. for the Catalan
 *          numbers.
 * https://oeis.org/A144186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of C_n / n! (C_n = binomial(2n,n)/(n+1))
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 95,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/build_primes_upto.h"

/* exponent of prime p in n! */
static inline idx_t exp_in_fact(idx_t n, idx_t p) {
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

seq_t a(idx_t n) {
    if (n == 0u) {
        return seq_one();
    }

    /* static cache of primes up to max needed */
    static uint64_t *primes = NULL;
    static size_t   primes_cnt = 0;
    static idx_t    primes_limit = 0;

    idx_t needed = 2 * n;
    if (needed > primes_limit) {
        free(primes);
        build_primes_upto(needed, &primes, &primes_cnt);
        primes_limit = needed;
    }

    seq_t result = seq_one();

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];

        /* exponent of p in C_n = binomial(2n,n)/(n+1) */
        idx_t eC = exp_in_fact(2 * n, p) - 2 * exp_in_fact(n, p);

        /* subtract exponent of p in (n+1) */
        idx_t m = n + 1;
        idx_t e_den_factor = 0;
        while (m % p == 0) {
            ++e_den_factor;
            m /= p;
        }
        if (eC < e_den_factor) {
            eC = 0;
        } else {
            eC -= e_den_factor;
        }

        /* exponent of p in n! (the denominator of the e.g.f.) */
        idx_t eDen = exp_in_fact(n, p);

        /* remaining exponent after canceling the gcd with n! */
        idx_t eResult = 0;
        if (eC > eDen) {
            eResult = eC - eDen;
        }

        if (eResult > 0) {
            seq_t pow_val = seq_pow((seq_t)p, (idx_t)eResult);
            result = seq_mul(result, pow_val);
        }
    }

    return result;
}
