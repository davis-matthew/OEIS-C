/*
 * A206033  a(1) =1; for n>=1: a(n) = product of numbers k <= sigma(n) such
 *          that k is not equal to sigma(d) for any divisor d of n where sigma
 *          = A000203.
 * https://oeis.org/A206033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏{ k : 1 ≤ k ≤ σ(n) and k ≠ σ(d) for any divisor d | n }
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 9, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empty product is defined as 1 (e.g. a(1)=1) */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* defined only for n ≥ 1, safety */

    /* Compute σ(n) */
    seq_t sigma_n = sigma_of(n);
    idx_t sigma_n_int = (idx_t)sigma_n;  /* fits for n ≤ 10^4 */

    /* Allocate flag array for values 0 … σ(n) */
    uint8_t *banned = (uint8_t *)calloc((size_t)sigma_n_int + 1, 1);
    if (banned == NULL) return 0;   /* allocation failure */

    /* Factor n into primes and exponents */
    uint64_t primes[64], exps[64];
    size_t prime_cnt = 0;
    factor_u64(n, primes, exps, &prime_cnt);

    /* Generate all divisors of n */
    uint64_t divs[256];
    size_t div_cnt = 1;
    divs[0] = 1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p   = primes[pi];
        unsigned int e = (unsigned int)exps[pi];  /* exponent fits in 32-bit */

        size_t old_cnt = div_cnt;
        uint64_t power = 1;

        for (unsigned int k = 1; k <= e; ++k) {
            power *= p;
            for (size_t j = 0; j < old_cnt; ++j) {
                divs[div_cnt++] = divs[j] * power;
            }
        }
    }

    /* Mark all σ(d) for divisors d of n */
    for (size_t i = 0; i < div_cnt; ++i) {
        seq_t sigma_d = sigma_of(divs[i]);
        if (sigma_d <= sigma_n) {
            banned[(idx_t)sigma_d] = 1;
        }
    }

    /* Compute the product of all k not banned */
    seq_t prod = 1;   /* empty product = 1 */
    for (idx_t k = 1; k <= sigma_n_int; ++k) {
        if (!banned[k]) {
            prod *= (seq_t)k;
        }
    }

    free(banned);
    return prod;
}
