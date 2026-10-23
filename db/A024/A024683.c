/*
 * A024683  a(n) is the number of ways prime(n) is a sum of two composite
 *          numbers r,s satisfying r < s.
 * https://oeis.org/A024683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of representations of prime(n) as a sum of two *
 *            composite numbers r,s with r < s. Computed by sieving primes and
 *            testing * each potential split. The sieve is enlarged
 *            dynamically as needed
 * Range:     u8 n <= 138, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Cached data for primes and primality flags.  All static so they persist
 * across calls.  The arrays are re‑computed when a larger bound is required. */
static uint64_t *primes      = NULL;   /* prime list (1‑based index stored 0‑based) */
static size_t   primes_cap   = 0;      /* allocated capacity for primes array   */
static size_t   primes_cnt   = 0;      /* current number of primes             */

static bool *is_prime     = NULL;     /* primality flags up to some limit      */
static size_t is_prime_cap = 0;       /* allocated size of is_prime array      */

/* Rebuild the sieve and prime list up to 'limit' (inclusive).  All previous
 * data is discarded.  The limit must be at least 2. */
static void recompute_up_to(size_t limit)
{
    /* free old data */
    free(is_prime);
    free(primes);
    is_prime          = NULL;
    primes            = NULL;
    primes_cap        = 0;
    primes_cnt        = 0;
    is_prime_cap      = 0;

    /* allocate primality flag array */
    is_prime = (bool*)malloc(limit + 1);
    if (!is_prime) abort();            /* out of memory */
    memset(is_prime, 1, limit + 1);     /* assume prime initially */
    is_prime[0] = is_prime[1] = 0;      /* 0 and 1 are not prime */

    /* simple Eratosthenes sieve */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* collect primes into dynamic array */
    size_t cap = limit / 10 + 10;      /* initial guess for capacity */
    primes      = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!primes) abort();
    primes_cap = cap;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!primes) abort();
            }
            primes[primes_cnt++] = i;
        }
    }
    is_prime_cap = limit + 1;
}

/* Ensure that the cached data reaches at least the n-th prime.  If not,
 * the sieve is rebuilt with a larger limit (heuristically n*15 or
 * double the previous bound). */
static void ensure_primes(idx_t n)
{
    if (primes_cnt >= (size_t)n) return;        /* already available */

    /* initial bound: for small n use a constant, otherwise a rough
     * multiple of n.  This is conservative enough for n up to tens
     * of millions. */
    size_t estimate = (n < 6) ? 15 : n * 15;

    /* keep enlarging until we have enough primes */
    while (primes_cnt < (size_t)n) {
        recompute_up_to(estimate);
        if (primes_cnt >= (size_t)n) break;
        estimate *= 2;                           /* double until sufficient */
    }
}

/* Main sequence function requested by the assignment. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset 1 */

    /* make sure we have at least the n‑th prime ready */
    ensure_primes(n);

    /* nth prime (primes array is 0‑based) */
    uint64_t prime_p = primes[n - 1];

    seq_t count = 0;

    /* iterate over possible first summands r from 4 up to floor((p‑1)/2) */
    uint64_t half = (prime_p - 1) / 2;
    for (uint64_t r = 4; r <= half; ++r) {
        uint64_t s = prime_p - r;
        /* both r and s must be composite (>=4 and !prime) */
        if (r >= 4 && !is_prime[(size_t)r] &&
            s >= 4 && !is_prime[(size_t)s])
            ++count;
    }

    return count;
}
