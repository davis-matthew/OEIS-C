/*
 * A146886  Duplicate of A006093.
 * https://oeis.org/A146886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = prime(n) – 1
 * Range:     i8 n <= 31, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (nth prime minus one,  i.e. all numbers one less than a prime)  */
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static idx_t *prime_cache = NULL;          /* primes[0] = 2, primes[1] = 3, ... */
static size_t prime_cache_size = 0;        /* number of primes stored */

/* extend prime_cache so that it contains at least n primes */
static void ensure_n_primes(idx_t n)
{
    if (prime_cache_size >= (size_t)n) return;  /* already have enough */

    /* upper bound for the nth prime: n*(ln n + ln ln n) + 3   (Rosser's theorem) */
    idx_t limit;
    if (n < 6) {
        limit = 15;                          /* small safety for the first few primes */
    } else {
        double dn = (double)n;
        limit = (idx_t)(dn * (log(dn) + log(log(dn))) + 3.0);
    }

    /* keep expanding the limit until we have enough primes */
    while (true) {
        /* sieve up to limit */
        bool *comp = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
        if (!comp) exit(1);   /* out of memory */

        size_t idx = 0;

        for (idx_t p = 2; p <= limit; ++p) {
            if (!comp[p]) {
                /* store prime if we still need more */
                if (idx < (size_t)n)
                    ++idx;
                /* mark multiples */
                if (p * p <= limit) {
                    for (idx_t m = p * p; m <= limit; m += p)
                        comp[m] = true;
                }
            }
        }

        size_t count = idx;  /* number of primes found ≤ limit */

        if (count >= (size_t)n) {
            /* collect primes up to n only */
            idx_t *newCache = (idx_t*)malloc(n * sizeof(idx_t));
            if (!newCache) exit(1);
            idx = 0;
            for (idx_t p = 2; p <= limit && idx < (size_t)n; ++p) {
                if (!comp[p]) newCache[idx++] = p;
            }

            free(prime_cache);
            prime_cache = newCache;
            prime_cache_size = n;
            free(comp);
            break;
        }

        free(comp);
        limit *= 2;   /* increase bound and try again */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; behaviour for n<1 is undefined */
    ensure_n_primes(n);
    /* prime_cache[n-1] holds the nth prime */
    return (seq_t)(prime_cache[n - 1] - 1);
}
