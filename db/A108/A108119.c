/*
 * A108119  Numbers k such that prime(k) and prime(k+1) are congruent to 1
 *          (mod 10).
 * https://oeis.org/A108119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th index k such that prime(k) and prime(k+1) both end with 1. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;                      /* undefined for n=0, guard */

    /* Initial sieve limit estimate: 500 times n gives more than enough primes for n≤10000. */
    idx_t limit = n * 500;
    if (limit < 200) limit = 200;                     /* small default for very small n */

    while (true) {
        /* Allocate sieve array (zeroed) */
        unsigned char *is_comp = (unsigned char *)calloc((size_t)limit + 1u, 1u);
        if (!is_comp) return (seq_t)0;                /* allocation failed */

        /* Temporary array for primes */
        size_t prime_cap = limit / 10 + 1u;
        uint64_t *primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!primes) {
            free(is_comp);
            return (seq_t)0;
        }
        size_t pcnt = 0;

        /* Sieve of Eratosthenes */
        for (idx_t i = 2; i <= limit; ++i) {
            if (!is_comp[i]) {
                if (pcnt == prime_cap) {
                    prime_cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                    if (!tmp) {
                        free(primes);
                        free(is_comp);
                        return (seq_t)0;
                    }
                    primes = tmp;
                }
                primes[pcnt++] = (uint64_t)i;
                if (i <= limit / i) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        is_comp[j] = 1;
                }
            }
        }

        /* Find the n-th adjacent pair ending in 1 */
        size_t pairs_found = 0;
        for (size_t i = 0; i + 1 < pcnt; ++i) {
            if ((primes[i] % 10u) == 1u && (primes[i + 1] % 10u) == 1u) {
                ++pairs_found;
                if (pairs_found == n) {
                    seq_t result = (seq_t)(i + 1);  /* indices are 1‑based */
                    free(primes);
                    free(is_comp);
                    return result;
                }
            }
        }

        /* Not enough pairs found – increase the limit and try again */
        free(primes);
        free(is_comp);
        limit <<= 1;
    }
}
