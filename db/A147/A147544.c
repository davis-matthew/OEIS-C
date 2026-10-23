/*
 * A147544  Coefficient of x^n in product (1+x)*Product_{j>=1} (1 +
 *          prime(j)*x^j).
 * https://oeis.org/A147544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (1+x) * ∏_{j≥1} (1 + prime(j) *
 *            x^j)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 41, u64 n <= 109, u128 n <= 303,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */

    /* Transform index to a size_t for array indexing */
    size_t m = (size_t)n;

    /* Get first m primes: prime(1)=2, prime(2)=3, ... */
    uint32_t *prime_list = NULL;
    size_t prime_cnt = 0;
    sieve_first_n_primes(m, &prime_list, &prime_cnt);

    /* Allocate array for polynomial coefficients up to degree m */
    seq_t *coef = (seq_t *)calloc(m + 1, sizeof(seq_t));
    if (!coef) {
        free(prime_list);
        return 0;
    }

    /* Start with (1 + x) factor */
    coef[0] = 1;
    if (m >= 1) coef[1] = 1;

    /* Multiply successively by (1 + prime(j) * x^j) for j = 1 … m */
    for (size_t j = 1; j <= m; ++j) {
        seq_t p = (seq_t)prime_list[j - 1];   /* prime(j) */
        for (size_t k = m; k >= j; --k) {
            coef[k] = coef[k] + p * coef[k - j];
        }
    }

    seq_t result = coef[m];        /* coefficient of x^n */

    /* Clean up allocated memory */
    free(coef);
    free(prime_list);

    return result;
}
