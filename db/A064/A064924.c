/*
 * A064924  If n is prime then a(n) = n; for the subsequent nonprime positions
 *          a(n + k) = (k+1)*n; then at the next prime position a new
 *          subsequence begins.
 * https://oeis.org/A064924
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nice, nonn, look
 * Formula:   a(n) = p * (n - p + 1) * where p is the largest prime ≤ n
 * Range:     u8 n <= 51, u16 n <= 2992, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ------- static data for prime sieve ------- */
static size_t sieve_limit = 0;          /* highest number sieved so far */
static uint8_t *is_prime = NULL;        /* 0=composite, 1=presumptive prime */
static uint32_t *primes = NULL;         /* list of found primes in ascending order */
static size_t prime_count = 0;          /* number of primes stored */
static size_t prime_cap   = 0;          /* allocated capacity of 'primes' */

/* ensure that every prime ≤ limit is known */
static void ensure_primes_up_to(idx_t limit)
{
    /* first allocation if needed */
    if (is_prime == NULL) {
        size_t init = 2;
        sieve_limit = 1;
        is_prime   = (uint8_t*)calloc(init + 1, sizeof(uint8_t)); /* indices 0..init */
        /* mark 0 and 1 as composite */
        is_prime[0] = 0;
        is_prime[1] = 0;
        primes     = NULL;
        prime_cap  = 0;
        prime_count = 0;
    }

    if ((size_t)limit <= sieve_limit)
        return;   /* already have enough */

    /* enlarge the sieve array */
    size_t new_limit = (size_t)limit;
    is_prime = (uint8_t*)realloc(is_prime, new_limit + 1);
    /* initialise new entries to presumptive prime (1) */
    for (size_t i = sieve_limit + 1; i <= new_limit; ++i)
        is_prime[i] = 1;

    /* mark composites in the new range using known primes */
    size_t sq = (size_t)sqrt((long double)new_limit);
    for (size_t i = 0; i < prime_count && primes[i] <= sq; ++i) {
        size_t p = primes[i];
        /* first multiple of p in [sieve_limit+1, new_limit] */
        size_t start = (sieve_limit + 1 + p - 1) / p * p;
        if (start < p * p) start = p * p;
        for (size_t j = start; j <= new_limit; j += p)
            is_prime[j] = 0;
    }

    /* collect newly discovered primes and store them in the list */
    for (size_t i = sieve_limit + 1; i <= new_limit; ++i) {
        if (is_prime[i]) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap == 0 ? 64 : prime_cap << 1;
                primes = (uint32_t*)realloc(primes, prime_cap * sizeof(uint32_t));
            }
            primes[prime_count++] = (uint32_t)i;
        }
    }

    /* ensure 0 and 1 remain composite */
    is_prime[0] = 0;
    is_prime[1] = 0;

    sieve_limit = new_limit;
}

/* ------- sequence function ------- */
seq_t a(idx_t n)
{
    /* undefined below the offset 2 */
    if (n < 2)
        return (seq_t)0;

    ensure_primes_up_to(n);

    /* binary search for the largest prime ≤ n */
    size_t lo = 0, hi = prime_count;   /* [lo,hi) */
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((idx_t)primes[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    size_t idx = lo - 1;   /* previous index is the needed prime */
    uint32_t p = primes[idx];

    /* compute a(n) = p * (n - p + 1) */
    seq_t diff = (seq_t)(n - (idx_t)p);
    seq_t result = (seq_t)((unsigned __int128)p * (unsigned __int128)(diff + (seq_t)1));

    return result;
}
