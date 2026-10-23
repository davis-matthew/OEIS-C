/*
 * A142236  Primes congruent to 39 mod 41.
 * https://oeis.org/A142236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A142236 – primes congruent to 39 (mod 41), offset 1.
    The function searches for primes in this residue class by sieving
    up to an estimated upper bound and caching the results.  When the
    cache is reused, the stored primes are kept in a plain uint64_t
    array, which is safe both for ordinary integer seq_t types and
    for arbitrary‑precision implementations (the cast at the end of
    a() performs the harmless conversion).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* Estimate an upper bound for the nth prime ≡ 39 (mod 41).
   The bound 39 + 40n·log n is generous enough for n ≤ 10 000
   and keeps the sieve size reasonable even for 64‑bit limits. */
static idx_t estimate_limit(idx_t n)
{
    if (n < 6)
        return 200;
    double dn = (double)n;
    double upper = dn * 41.0 * (log(dn) + log(log(dn)));
    return (idx_t)(upper + 100.0);   /* safety margin */
}

/* Cache of primes ≡ 39 (mod 41).  Stored as uint64_t for safe
   construction in all build modes; cast to seq_t on return. */
static uint64_t *prime_cache = NULL;   /* array of primes */
static idx_t      prime_count = 0;     /* number of cached primes */

/* Ensure that prime_cache contains at least n primes. */
static void
ensure_cache(idx_t n)
{
    if (prime_count >= n)
        return;   /* already enough */

    /* Compute a sieve up to the required limit, re‑sieving if needed. */
    idx_t limit = estimate_limit(n);

    while (prime_count < n) {
        /* Allocate a boolean array for the sieve. */
        size_t L = (size_t)limit;
        char *is_prime = (char *)malloc(L + 1);
        if (!is_prime)
            return;   /* memory failure – leave cache unchanged */

        memset(is_prime, 1, L + 1);  /* assume prime */
        if (L >= 0) is_prime[0] = 0;
        if (L >= 1) is_prime[1] = 0;

        size_t sq = (size_t)sqrt((double)L);
        for (size_t p = 2; p <= sq; ++p) {
            if (is_prime[p]) {
                for (size_t m = p * p; m <= L; m += p)
                    is_prime[m] = 0;
            }
        }

        /* Re‑allocate the cache array each time we rebuild it.
           All existing data is discarded to keep things simple. */
        free(prime_cache);
        prime_cache = NULL;
        prime_count = 0;

        size_t capacity = 256;
        prime_cache = (uint64_t *)malloc(capacity * sizeof(uint64_t));
        if (!prime_cache) {
            free(is_prime);
            return;   /* memory failure */
        }

        uint64_t count = 0;
        for (size_t i = 2; i <= L && count < n; ++i) {
            if (is_prime[i] && (i % 41ULL) == 39ULL) {
                if (count >= capacity) {
                    capacity <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                                        capacity * sizeof(uint64_t));
                    if (!tmp) {
                        free(prime_cache);
                        prime_cache = NULL;
                        prime_count = 0;
                        break;
                    }
                    prime_cache = tmp;
                }
                prime_cache[count++] = (uint64_t)i;
            }
        }
        free(is_prime);
        prime_count = count;

        if (prime_count < n)      /* need more primes – enlarge limit */
            limit <<= 1;
    }
}

/* Public function – returns the nth prime ≡ 39 (mod 41), offset 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1 */

    ensure_cache(n);
    if (n - 1 < prime_count)
        return (seq_t)prime_cache[n - 1];
    return 0;   /* should not happen */
}
