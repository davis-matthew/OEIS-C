/*
 * A139302  a(n) = 2^(2p - 1)/16, where p is the n-th Mersenne prime
 *          A000668(n).
 * https://oeis.org/A139302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2p-5) where p is the n-th Mersenne prime (A000668(n))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime, is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* is_prime(idx_t) */
#include "../utils/is_prime_big.h"      /* is_prime_big(seq_t) */
#include "../utils/pow2.h"              /* pow2(idx_t) */

seq_t a(idx_t n)
{
    /* Static storage for known Mersenne prime exponents */
    static idx_t *exponents = NULL;
    static size_t exps_cnt = 0;   /* counted Mersenne primes found so far */
    static idx_t  tested = 2;     /* smallest exponent to test next */

    if (n == 0)
        return (seq_t)0;

    /* Expand exponent array on first call */
    if (exponents == NULL) {
        exponents = (idx_t *)malloc(sizeof(idx_t) * 16);
        if (!exponents) return (seq_t)0;
        exps_cnt = 0;
    }

    /* Find n-th Mersenne prime exponent */
    while (exps_cnt < (size_t)n) {
        /* Test candidate exponent for primality */
        if (is_prime(tested)) {
            /* 2^tested - 1 is candidate Mersenne prime */
            seq_t mp = pow2(tested) - (seq_t)1;
            if (is_prime_big(mp)) {
                /* Store exponent */
                if (exps_cnt == 16) {
                    /* Grow array if necessary */
                    idx_t *tmp = (idx_t *)realloc(exponents, sizeof(idx_t) * (exps_cnt + 16));
                    if (!tmp) {
                        free(exponents);
                        exponents = NULL;
                        return (seq_t)0;
                    }
                    exponents = tmp;
                }
                exponents[exps_cnt++] = tested;
            }
        }
        tested++; /* next exponent to examine */
    }

    /* Compute a(n) = 2^(2p-5) = 2^(2^(e+1)-7), where e is the exponent */
    idx_t e = exponents[n - 1];
    /* Ensure we can shift by (e+1) within 64 bits */
    if (e + 1 >= 64) {
        /* Exponent too large for pow2(); return 0 as out-of-range */
        return (seq_t)0;
    }

    idx_t exp = ((idx_t)1 << (e + 1)) - 7;
    return pow2(exp);
}
