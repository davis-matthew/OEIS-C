/*
 * A104181  Let f(n)=mod(prime(n),12); then a(n) = binomial(prime(12),f(n)).
 * https://oeis.org/A104181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(37, prime(n) % 12) (prime(n) = the n-th prime,
 *            37 = prime(12))
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom37
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/binom37.h"

static uint32_t *prime_cache = NULL;   /* array of primes, 0‑based      */
static idx_t prime_cache_size = 0;     /* number of primes cached      */

/* ------------------------------------------------------------ */
/* Return the n‑th prime (n ≥ 1).  The function expands the
   local prime cache if necessary.  It uses a simple sieve with
   a conservative upper bound:  for n ≥ 6, pₙ < n*(log n + log log n)  */
static bool ensure_primes(idx_t n)
{
    if (prime_cache_size >= n)
        return true;               /* already available               */

    uint64_t upper;
    if (n < 6) {
        upper = 15;   /* small constant that contains the first 5 primes */
    } else {
        double dn = (double)n;
        upper = (uint64_t)(dn * (log(dn) + log(log(dn)))) + 3;
    }

    /* If the estimate is too low we double it until it contains n primes. */
    while (true) {
        size_t len = (size_t)(upper + 1);
        uint8_t *is_comp = (uint8_t *)calloc(len, 1);
        if (!is_comp)
            return false;          /* allocation failure */

        is_comp[0] = is_comp[1] = 1;
        uint64_t sqrtL = (uint64_t)(sqrt((double)upper) + 1.0);

        for (uint64_t i = 2; i <= sqrtL; ++i) {
            if (!is_comp[i]) {
                for (uint64_t j = i * i; j <= upper; j += i)
                    is_comp[j] = 1;
            }
        }

        /* Count primes and store them if we have enough */
        idx_t prime_count = 0;
        for (uint64_t i = 2; i <= upper; ++i)
            if (!is_comp[i])
                ++prime_count;

        if (prime_count >= n) {
            /* Allocate cache for all primes found */
            uint32_t *new_cache = (uint32_t *)realloc(prime_cache,
                                                      prime_count * sizeof(uint32_t));
            if (!new_cache) {
                free(is_comp);
                return false;
            }
            prime_cache   = new_cache;
            prime_cache_size = prime_count;

            idx_t pos = 0;
            for (uint64_t i = 2; i <= upper; ++i)
                if (!is_comp[i])
                    prime_cache[pos++] = (uint32_t)i;

            free(is_comp);
            return true;
        }

        /* Not enough primes – enlarge the bound and try again */
        free(is_comp);
        upper = upper * 2 + 10;      /* safety margin */
    }
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n = 0 */

    if (!ensure_primes(n))
        return 0;                      /* allocation or computation failure */

    uint32_t p   = prime_cache[n - 1];
    uint32_t k   = p % 12U;
    return binom37(k);
}
