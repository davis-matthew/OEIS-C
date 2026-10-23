/*
 * A120079  Unsigned row sums of triangle A120078.
 * https://oeis.org/A120079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((lcm(1..n)/n)^2) * (2*n^2 - 1) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   build_primes_upto, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   lcm(1..n) = product over primes p <= n of p^⌊log_p(n)⌋   */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/build_primes_upto.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"

/* external linkage as required */
seq_t a(idx_t n)
{
    /* static state for caching primes and factor exponents */
    static uint64_t *primes         = NULL;   /* list of primes up to current limit */
    static size_t     primes_cnt    = 0;      /* number of primes */
    static idx_t      primes_limit  = 0;      /* largest n processed so far */

    static int   *exp_n_cache        = NULL;  /* cache of exponents of n in each prime   */
    static size_t exp_cache_size      = 0;     /* size of that cache                        */

    if (n < 1)               /* sequence is defined only for n >= 1 */
        return (seq_t)0;

    /* ------------------------------------------------------------------ */
    /* 1.  Ensure we have all primes up to n                              */
    /* ------------------------------------------------------------------ */
    if (n > primes_limit) {
        if (primes) free(primes);              /* free old list, if any              */
        build_primes_upto(n, &primes, &primes_cnt);
        primes_limit = n;
    }

    /* ------------------------------------------------------------------ */
    /* 2.  Ensure the exponent cache is large enough                       */
    /* ------------------------------------------------------------------ */
    if (primes_cnt > exp_cache_size) {
        if (exp_n_cache) free(exp_n_cache);
        exp_n_cache = (int *)calloc(primes_cnt, sizeof(int));
        exp_cache_size = primes_cnt;
    }

    /* ------------------------------------------------------------------ */
    /* 3.  Factor n once into the cache exp_n_cache[pi]                    */
    /* ------------------------------------------------------------------ */
    memset(exp_n_cache, 0, primes_cnt * sizeof(int));

    idx_t tmp = n;
    for (size_t pi = 0; pi < primes_cnt && primes[pi] * primes[pi] <= tmp; ++pi) {
        uint64_t p = primes[pi];
        while (tmp % p == 0) {
            tmp /= p;
            exp_n_cache[pi]++;
        }
    }
    if (tmp > 1) {                     /* remaining prime > sqrt(n) */
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            if (primes[pi] == tmp) {
                exp_n_cache[pi]++;
                break;
            }
        }
    }

    /* ------------------------------------------------------------------ */
    /* 4.  Compute product of primes p^(⌊log_p(n)⌋ - exp_in_n)             */
    /* ------------------------------------------------------------------ */
    seq_t lcm_over_n = (seq_t)1;

    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        if (p > n) break;                /* primes > n are not needed */

        /* floor(log_p(n))  */
        unsigned expL = 0;
        idx_t pw = p;
        while (pw <= n) {
            expL++;
            pw *= p;
        }

        int diff = (int)expL - exp_n_cache[pi];
        if (diff > 0) {
            /* multiply by p^diff */
            seq_t mult = (seq_t)1;
            for (int k = 0; k < diff; ++k)
                mult = mul_small(mult, (unsigned)p);
            lcm_over_n = seq_mul(lcm_over_n, mult);
        }
    }

    /* ------------------------------------------------------------------ */
    /* 5.  Compute (L_n / n)^2                                            */
    /* ------------------------------------------------------------------ */
    seq_t lcm_sq = seq_mul(lcm_over_n, lcm_over_n);

    /* ------------------------------------------------------------------ */
    /* 6.  Multiply by (2*n^2 - 1)                                        */
    /* ------------------------------------------------------------------ */
    seq_t mult_factor = (seq_t)((seq_t)2 * n * n - 1);   /* (2*n^2 - 1) */

    return seq_mul(lcm_sq, mult_factor);
}
