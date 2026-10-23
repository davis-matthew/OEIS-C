/*
 * A048165  Expansion of Product_{k > 0} 1/(1 + x^prime(k)).
 * https://oeis.org/A048165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{p prime} 1/(1 + x^p)
 * Range:     i8 n <= 56, i16 n <= 144, i32 n <= 439, i64 n <= 1541,
 *            i128 n <= 5993, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses an in‑place DP:            */
/*   a[0] = 1; a[i] = 0 for i>0                   */
/*   For each prime p:  for i = p .. n   a[i] -= a[i‑p]   */
/* This produces the required sequence for any n >= 0.    */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"

seq_t a(idx_t n)
{
    /* allocate coefficient array */
    seq_t *coeff = alloc_seq((size_t)n + 1);
    coeff[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) coeff[i] = (seq_t)0;

    /* sieve primes up to n */
    if (n >= 2) {
        bool *is_prime = (bool*)calloc((size_t)n + 1, sizeof(bool));
        for (idx_t i = 2; i <= n; ++i) is_prime[i] = true;
        for (idx_t p = 2; p * p <= n; ++p) {
            if (is_prime[p]) {
                for (idx_t j = p * p; j <= n; j += p)
                    is_prime[j] = false;
            }
        }

        /* count primes */
        idx_t prime_cnt = 0;
        for (idx_t i = 2; i <= n; ++i)
            if (is_prime[i]) ++prime_cnt;

        /* store primes in an array */
        idx_t *primes = (idx_t*)malloc((size_t)prime_cnt * sizeof(idx_t));
        idx_t pc = 0;
        for (idx_t i = 2; i <= n; ++i)
            if (is_prime[i]) primes[pc++] = i;

        /* in‑place DP: for each prime, update coefficients */
        for (idx_t pi = 0; pi < prime_cnt; ++pi) {
            idx_t p = primes[pi];
            for (idx_t i = p; i <= n; ++i)
                coeff[i] = coeff[i] - coeff[i - p];
        }

        free(primes);
        free(is_prime);
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
}
