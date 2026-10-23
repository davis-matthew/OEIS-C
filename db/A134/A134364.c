/*
 * A134364  a(n) = Product_{k=1..A061395(n)} prime(k)^|A134363(n,k)| for n >=
 *          2; a(1) = 1.
 * https://oeis.org/A134364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..A061395(n)} prime(k) ^ |A134363(n,k)| where
 *            A134363(n,k) is defined by the exponents of the prime factor
 *            factorisation of n as e1 = v2(n), ek = v_{p_k}(n) -
 *            v_{p_{k-1}}(n) (k ≥ 2), and A061395(n) is the index of the
 *            largest prime divisor of n
 * Range:     u8 n <= 39, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   build_primes, factor_u64, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for all n ≥ 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Helper functions supplied by the build system */
#include "../utils/build_primes.h"   /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/factor_u64.h"     /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/seq_pow.h"       /* seq_t seq_pow(seq_t base, idx_t exp); */
#include "../utils/seq_mul.h"       /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/seq_one.h"       /* seq_t seq_one(void); */

/* ---------------------------------------------------------------------*/
/* Compute a(n) as defined by OEIS A134364                                 */
/* ---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n <= 1)
        return seq_one();

    /* ------------------------------------------------------------------
       Step 1: factorise n.                                              */
    /* Allocate a buffer that is large enough for any 64‑bit integer
       (a 64‑bit number can have at most 16 distinct prime factors).       */
    uint64_t *factors   = (uint64_t *)malloc(32 * sizeof(uint64_t));
    uint64_t *exponents = (uint64_t *)malloc(32 * sizeof(uint64_t));
    size_t   nfactors   = 0;
    if (!factors || !exponents) {
        /* Allocation failure – fall back to 1. */
        free(factors);
        free(exponents);
        return seq_one();
    }

    factor_u64((uint64_t)n, factors, exponents, &nfactors);

    /* -----------------------------------------------------------------
       Step 2: determine the largest prime divisor, then build a full
       list of primes up to that value.                                */
    uint64_t max_prime = 0;
    for (size_t i = 0; i < nfactors; ++i)
        if (factors[i] > max_prime) max_prime = factors[i];

    idx_t *prime_list = NULL;
    size_t prime_cnt  = 0;
    build_primes(max_prime, &prime_list, &prime_cnt);   /* prime_list[0] = 2 */

    /* -----------------------------------------------------------------
       Step 3: create an array of exponents for all primes up to
       the largest prime divisor.                                      */
    int *prime_exps = (int *)calloc(prime_cnt, sizeof(int));
    if (!prime_exps) {
        free(factors);
        free(exponents);
        free(prime_list);
        return seq_one();
    }

    /* Insert the known exponents into the array.                       */
    for (size_t i = 0; i < nfactors; ++i) {
        uint64_t p = factors[i];
        for (size_t j = 0; j < prime_cnt; ++j) {
            if (prime_list[j] == p) {
                prime_exps[j] = (int)exponents[i];
                break;
            }
        }
    }

    /* -----------------------------------------------------------------
       Step 4: compute the product according to the definition.        */
    seq_t result = seq_one();

    for (size_t k = 0; k < prime_cnt; ++k) {
        int e;
        if (k == 0)
            e = prime_exps[0];
        else
            e = prime_exps[k] - prime_exps[k - 1];

        if (e == 0) continue;

        int abs_e = e < 0 ? -e : e;
        seq_t base = (seq_t)prime_list[k];
        seq_t power = seq_pow(base, (idx_t)abs_e);
        result = seq_mul(result, power);
    }

    /* -----------------------------------------------------------------
       Step 5: free allocated memory and return.                       */
    free(factors);
    free(exponents);
    free(prime_exps);
    free(prime_list);

    return result;
}
