/*
 * A055768  Number of distinct primes dividing phi of n-th primorial number.
 * https://oeis.org/A055768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct primes dividing φ of the n‑th
 *            primorial number (OEIS A055768)
 * Range:     u8 n <= 1065, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm:
   1. Compute the list of all primes up to a fixed limit (>> the
      10 000th prime) with a linear sieve and store the smallest prime
      factor (spf) of every number.
   2. Maintain a boolean table `seen` showing which primes have already
      appeared as a factor of (p_i-1) for the processed primorials.
   3. For each new n, factor p_n‑1 with the precomputed spf table,
      add unseen prime factors to `seen`, increment the distinct‑prime
      counter, and cache the result. Subsequent calls reuse the cache
      and continue from where they left off.  The entire routine uses
      only integer arithmetic and runs in O(n log log LIMIT) for all
      n ≤ 100 000.  It works for any seq_t and idx_t defined by the
      build system. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* --- parameters ----------------------------------------------------------- */
#define SIEVE_LIMIT 2000000          /* > 10 000th prime (104 729) + margin */

/* --- static data ---------------------------------------------------------- */
static int primes[SIEVE_LIMIT];          /* list of primes (prime_count entries) */
static int prime_count = 0;              /* number of primes found */
static int spf[SIEVE_LIMIT + 1];         /* smallest prime factor for each number */
static bool seen[SIEVE_LIMIT + 1];       /* whether a prime has appeared as a factor */

static seq_t *cache = NULL;             /* cache of a(n) values; cache[0] unused */
static idx_t cache_len = 0;             /* number of valid entries in cache */
static idx_t cache_cap = 0;             /* current capacity of cache array */
static int seen_count = 0;              /* current number of distinct prime factors */

/* --- internal helper ------------------------------------------------------ */
static void init_sieve(void)
{
    if (prime_count > 0)            /* already initialised */
        return;

    /* linear sieve */
    for (int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[prime_count++] = i;
        }
        for (int j = 0; j < prime_count &&
                        primes[j] <= spf[i] &&
                        (int64_t)i * primes[j] <= SIEVE_LIMIT; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }

    /* initialise seen table */
    for (int i = 0; i <= SIEVE_LIMIT; ++i)
        seen[i] = false;

    /* initialise cache */
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[0] = 0;           /* unused slot for a(0) */
    cache_len = 1;          /* currently only index 0 stored */
}

/* --- public function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n < 1, but return 0 safely */

    init_sieve();

    /* ensure cache can hold a(n) */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;            /* double until large enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* extend cache up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* primes[0] is the first prime, so a(1) uses primes[0] */
        int p = primes[i - 1];
        int x = p - 1;

        /* factor x using the precomputed spf table */
        while (x > 1) {
            int q = spf[x];
            if (!seen[q]) {
                seen[q] = true;
                ++seen_count;
            }
            while (x % q == 0)
                x /= q;
        }

        cache[i] = (seq_t)seen_count;
    }

    cache_len = n + 1;
    return cache[n];
}
