/*
 * A218829  Number of ordered ways to write n = k + m with k > 0 and m > 0
 *          such that prime(k) + 2 and prime(prime(m)) + 2 are both prime.
 * https://oeis.org/A218829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = # of ordered pairs (k,m) with k>0, m>0, k+m=n,
 *   such that prime(k)+2 is prime and prime(prime(m))+2 is prime.
 *   (prime(n) denotes the n-th prime, starting with prime(1)=2.)
 *
 * The implementation uses a precomputed prime sieve up to a fixed limit
 * (5 000 000) which is sufficient for all n <= 10 000, the usual range
 * for this sequence.  For each query it performs a single O(n) scan.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Maximum limit for the prime sieve: sufficient for all n up to about 10 000. */
static const size_t PRIME_SIEVE_LIMIT = 5000000;

/* Global data (initialized on first call to a). */
static bool      *global_is_prime      = NULL;   /* bool array [0..PRIME_SIEVE_LIMIT]   */
static uint32_t  *global_primes        = NULL;   /* list of primes, 1-indexed          */
static size_t     global_prime_count   = 0;      /* number of primes, excluding dummy  */
static bool       global_initialized   = false;   /* init flag                          */

/* ---------- Prime sieve and primes list ---------- */
static void init_prime_sieve(void)
{
    if (global_initialized)
        return;

    /* Allocate and init boolean sieve */
    global_is_prime = (bool*)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(bool));
    if (global_is_prime == NULL) abort();
    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i)
        global_is_prime[i] = true;
    global_is_prime[0] = global_is_prime[1] = false;

    /* Sieve of Eratosthenes */
    for (size_t p = 2; p * p <= PRIME_SIEVE_LIMIT; ++p) {
        if (global_is_prime[p]) {
            for (size_t q = p * p; q <= PRIME_SIEVE_LIMIT; q += p)
                global_is_prime[q] = false;
        }
    }

    /* Build primes list (1‑indexed, primes[0] = 0 dummy). */
    /* Estimate number of primes (~LIMIT / log(LIMIT)), allocate buffer. */
    size_t estimate = PRIME_SIEVE_LIMIT / 10;  /* 500 000 > 348 513 */
    global_primes = (uint32_t*)malloc((estimate + 2) * sizeof(uint32_t));
    if (global_primes == NULL) abort();
    global_primes[0] = 0;            /* dummy for 1‑based indexing */
    global_prime_count = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (global_is_prime[i]) {
            ++global_prime_count;
            if (global_prime_count + 1 > estimate) {
                /* reallocate larger buffer if needed */
                estimate *= 2;
                global_primes = (uint32_t*)realloc(global_primes,
                                                   (estimate + 2) * sizeof(uint32_t));
                if (global_primes == NULL) abort();
            }
            global_primes[global_prime_count] = (uint32_t)i;
        }
    }
    global_initialized = true;
}

/* ---------- The required function ---------- */
seq_t a(idx_t n)
{
    /* Ensure primes are ready */
    init_prime_sieve();

    if (n <= 2)
        return 0;

    seq_t count = 0;

    for (idx_t k = 1; k <= n - 1; ++k) {
        idx_t m = n - k;

        /* Check condition for k: prime(k) + 2 is prime */
        uint32_t pk = global_primes[k];
        if (!global_is_prime[(size_t)(pk + 2)])
            continue;

        /* Check condition for m: prime(prime(m)) + 2 is prime */
        uint32_t pm = global_primes[m];          /* prime(m) */
        /* pm is guaranteed to be <= PRIME_SIEVE_LIMIT */
        /* need the pm‑th prime: global_primes[pm] */
        if (pm >= global_prime_count)  /* safety guard */
            continue;
        uint32_t p2m = global_primes[pm];       /* prime(prime(m)) */
        if (!global_is_prime[(size_t)(p2m + 2)])
            continue;

        ++count;
    }

    return count;
}
