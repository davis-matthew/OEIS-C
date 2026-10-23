/*
 * A142544  Primes congruent to 14 mod 53.
 * https://oeis.org/A142544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime that is congruent to 14 (mod 53)
 * Range:     u8 n <= 2, u16 n <= 131, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t *primes_uint64 = NULL; /* primes congruent to 14 mod 53 */
static size_t primes_cnt  = 0;          /* number of stored primes    */
static size_t primes_cap  = 0;          /* allocated capacity        */
static idx_t  sieve_limit = 0;          /* last sieve upper bound    */

/* Rebuild all primes ≤ limit and keep only those ≡ 14 (mod 53) */
static void recompute_primes(idx_t limit)
{
    /* free any previous prime list */
    free(primes_uint64);
    primes_uint64 = NULL;
    primes_cnt    = 0;
    primes_cap    = 0;

    /* allocate sieve array (boolean array) */
    unsigned char *sieve = (unsigned char*)calloc((size_t)(limit + 1), 1);
    if (!sieve)
        return; /* allocation failed – nothing to do */

    /* simple Eratosthenes sieve */
    for (idx_t p = 2; p * p <= limit; ++p)
        if (!sieve[(size_t)p])
            for (idx_t q = p * p; q <= limit; q += p)
                sieve[(size_t)q] = 1;

    /* collect primes ≡ 14 (mod 53) */
    for (idx_t p = 2; p <= limit; ++p)
    {
        if (!sieve[(size_t)p] && (p % 53 == 14))
        {
            if (primes_cnt == primes_cap)
            {
                size_t new_cap = primes_cap ? primes_cap * 2 : 8;
                uint64_t *tmp = (uint64_t*)realloc(primes_uint64, new_cap * sizeof(uint64_t));
                if (!tmp)
                {
                    free(primes_uint64);
                    primes_uint64 = NULL;
                    primes_cnt    = primes_cap = 0;
                    free(sieve);
                    return; /* allocation failed – abort */
                }
                primes_uint64 = tmp;
                primes_cap    = new_cap;
            }
            primes_uint64[primes_cnt++] = (uint64_t)p;
        }
    }

    free(sieve);
}

/* Return the n‑th term of A142544 (indices start at 1). */
seq_t a(idx_t n)
{
    if (n < 1)                           /* undefined for n < 1 */
        return 0;

    /* Grow the sieve until we have at least n required primes. */
    while (primes_cnt < (size_t)n)
    {
        idx_t new_limit = sieve_limit ? sieve_limit << 1 : 100;
        recompute_primes(new_limit);
        sieve_limit = new_limit;
    }

    return (seq_t)primes_uint64[n - 1];
}
