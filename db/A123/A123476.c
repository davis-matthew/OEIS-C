/*
 * A123476  a(n) = (n!)^2/phi(n!), where phi is Euler's totient function.
 * https://oeis.org/A123476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2/phi(n!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Handle offset 1: a(1) = 1 */
    if (n < 1) return (seq_t)1;

    size_t prime_cnt = 0;
    uint64_t *primes = NULL;
    build_primes_upto(n, &primes, &prime_cnt);

    idx_t  *e   = NULL;
    int64_t *exp = NULL;
    if (prime_cnt > 0) {
        e   = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
        exp = (int64_t *)malloc(prime_cnt * sizeof(int64_t));
    }

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];

        /* exponent of p in n! */
        idx_t ee = 0;
        uint64_t m = n;
        while (m) {
            m /= p;
            ee += m;
        }
        e[i] = ee;
        exp[i] = (int64_t)ee + 1;          /* start with p^{e+1} */

        /* subtract contribution of (p‑1) */
        uint64_t rem = p - 1;
        if (rem > 0) {
            uint64_t fac[32];
            uint64_t exs[32];
            size_t cnt_f = 0;
            factor_u64(rem, fac, exs, &cnt_f);
            for (size_t j = 0; j < cnt_f; ++j) {
                uint64_t q = fac[j];
                uint64_t qe = exs[j];
                /* find the index of q in primes array */
                for (size_t k = 0; k < prime_cnt; ++k) {
                    if (primes[k] == q) {
                        exp[k] -= (int64_t)qe;
                        break;
                    }
                }
            }
        }
    }

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (exp[i] > 0) {
            seq_t term = seq_pow((seq_t)primes[i], (idx_t)exp[i]);
            result = seq_mul(result, term);
        }
    }

    free(primes);
    free(e);
    free(exp);
    return result;
}
