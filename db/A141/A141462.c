/*
 * A141462  Transformed nonprime products of prime factors of the composites,
 *          the largest prime decremented by 2, the smallest by 1.
 * https://oeis.org/A141462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k*(pmax-2)*(pmin-1)/(pmin*pmax) where k is the n-th
 *            composite, * pmin, pmax are the smallest and largest prime
 *            factors of k; * the sequence lists this transformed product only
 *            when it is not prime (zero * is included)
 * Range:     u8 n <= 9, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   factor_u64, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"
#include "../utils/factor_u64.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Static storage for computed terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static idx_t terms_cnt = 0;   /* number of output terms stored */
    static uint64_t next_k = 4;   /* next composite to examine */

    /* Ensure we have at least n terms computed */
    if (n > terms_cnt) {
        /* Grow array capacity if necessary */
        size_t new_cap = terms_cap ? terms_cap : 128;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms = tmp;
        terms_cap = new_cap;

        /* Helper buffers for factorization */
        static uint64_t primes[64];
        static uint64_t exps[64];
        size_t prime_cnt;

        while (terms_cnt < n) {
            /* Find next composite k */
            while (is_prime_small((seq_t)next_k)) ++next_k;
            uint64_t k = next_k;
            ++next_k;

            /* Factor k */
            factor_u64(k, primes, exps, &prime_cnt);
            uint64_t pmin = UINT64_MAX, pmax = 0;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = primes[i];
                if (p < pmin) pmin = p;
                if (p > pmax) pmax = p;
            }

            /* Compute transformed product */
            seq_t num = (seq_t)k *
                        (seq_t)(pmax - 2) *
                        (seq_t)(pmin - 1);
            seq_t denom = (seq_t)pmin * (seq_t)pmax;
            seq_t val = denom ? num / denom : 0;

            /* Store only if nonprime (zero is included) */
            if (!is_prime_big(val)) {
                terms[terms_cnt++] = val;
            }
        }
    }

    return terms[n - 1];
}
