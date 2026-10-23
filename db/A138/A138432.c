/*
 * A138432  a(n) = ((n-th prime)^5-(n-th prime)^3)/2.
 * https://oeis.org/A138432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n)^5 - (p_n)^3) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* p_n is the n‑th prime, offset 1. The function uses the helper
   sieve_first_n_primes to obtain the required prime efficiently. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *primes_cache = NULL;   /* 1‑based array of primes */
static idx_t   primes_cache_max = 0;    /* current maximum n cached     */
static size_t  primes_cache_cnt = 0;    /* number of primes cached (==n)*/

seq_t a(idx_t n)
{
    if (n == 0) {                 /* undefined; return 0 for safety   */
        return 0;
    }

    /* Ensure at least n primes are cached */
    if (n > primes_cache_max) {
        if (primes_cache != NULL) {
            free(primes_cache);           /* free previous allocation */
        }
        size_t cnt = 0;
        sieve_first_n_primes(n, &primes_cache, &cnt);
        primes_cache_max = n;
        primes_cache_cnt = cnt;
    }

    /* Retrieve the n-th prime (array is 0‑based) */
    seq_t p = (seq_t)primes_cache[n - 1];

    /* Compute p^3, p^5 and the formula */
    seq_t p2 = p * p;        /* p^2 */
    seq_t p3 = p2 * p;       /* p^3 */
    seq_t p5 = p3 * p2;      /* p^5 = p^3 * p^2 */

    return (p5 - p3) / (seq_t)2;
}
