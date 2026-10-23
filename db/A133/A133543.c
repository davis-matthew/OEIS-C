/*
 * A133543  Sum of seventh powers of five consecutive primes.
 * https://oeis.org/A133543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^7 + prime(n+1)^7 + prime(n+2)^7 + prime(n+3)^7 +
 *            prime(n+4)^7
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 84, u128 n <= 1000,
 *            big any n
 * Helpers:   _prime_limit_for_idx, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities available from the framework */
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/seq_pow.h"

static uint32_t *primes = NULL;    /* array of primes, 1‑based index */
static size_t prime_cnt = 0;       /* number of primes stored   */
static size_t prime_cap = 0;       /* current capacity         */
static size_t prime_limit = 0;     /* upper bound of primes computed */

/* Simple Eratosthenes sieve generating all primes <= limit. */
static void compute_primes_upto(size_t limit)
{
    /* Allocate a sieve array (1 = composite, 0 = prime candidate) */
    uint8_t *is_comp = (uint8_t*)calloc(limit + 1, 1);
    if (!is_comp) return;  /* allocation failure – ignore for brevity */

    size_t i, j;
    size_t sq = (size_t)sqrt(limit);
    for (i = 2; i <= sq; ++i)
        if (!is_comp[i]) {
            for (j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }

    /* Count primes */
    size_t np = 0;
    for (i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++np;

    /* Allocate new primes array if needed */
    if (np > prime_cap) {
        uint32_t *newp = (uint32_t*)realloc(primes, np * sizeof(uint32_t));
        if (!newp) { /* allocation failure – ignore for brevity */
            free(is_comp);
            return;
        }
        primes = newp;
        prime_cap = np;
    }

    /* Store primes */
    size_t idx = 0;
    for (i = 2; i <= limit; ++i)
        if (!is_comp[i]) primes[idx++] = (uint32_t)i;

    prime_cnt = np;
    prime_limit = limit;
    free(is_comp);
}

seq_t a(idx_t n)
{
    /* n is 1‑based; need primes n .. n+4 */
    if (n == 0) return 0;  /* undefined for 0, but guard */

    /* Determine required bound for nth prime + 4 */
    idx_t need_idx = n + 4;
    /* retrieve an upper bound for the need_idx‑th prime */
    size_t bound = _prime_limit_for_idx(need_idx) + 1; /* safety margin */

    /* recompute primes if necessary */
    if (prime_cnt < (size_t)need_idx || prime_limit < bound)
        compute_primes_upto(bound);

    /* Safety check: make sure we have enough primes */
    if (prime_cnt < (size_t)need_idx)
        return 0;  /* failure; should not happen for reasonable n */

    seq_t sum = 0;
    for (int i = 0; i < 5; ++i) {
        uint32_t p = primes[n - 1 + i];
        sum += seq_pow((seq_t)p, (idx_t)7);
    }
    return sum;
}
