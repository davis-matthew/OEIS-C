/*
 * A088680  a(n) = prime(2n+1) - prime(2n).
 * https://oeis.org/A088680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = prime(2*n+1) – prime(2*n). The function builds * primes
 *            on demand using trial division and keeps them cached, so any *
 *            sequence of calls works in O(n^2 / log n) total time, which is *
 *            well below the limits for n up to about 10 000
 * Range:     u8 n <= 27, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a static cache of known primes.  The first
 * prime is 2, subsequent primes are detected by trial division using
 * the primes already in the cache.  When a new prime index is
 * requested, the cache is extended until it contains at least that
 * many primes. */
#include <stdlib.h>

/* external integer types supplied by the build */
extern seq_t a(idx_t n);      /* function to be defined below */

static seq_t *prime_cache = NULL;   /* array of seen primes (1‑based) */
static idx_t  prime_count = 0;      /* number of primes in cache */
static seq_t  last_checked = 2;     /* last integer examined for primality */
static size_t prime_cap   = 0;       /* allocated capacity of prime_cache */

/* Ensure the cache holds at least needed_index primes (1‑based). */
static void ensure_primes(idx_t needed_index)
{
    /* Fast return when cache already large enough */
    if (prime_count >= needed_index) return;

    /* Grow cache capacity if necessary */
    if (prime_cap < prime_count + 10) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 32;
        if (new_cap < prime_count + 10) new_cap = prime_count + 10;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort */
            abort();
        }
        prime_cache = tmp;
        prime_cap = new_cap;
    }

    /* If cache empty, prime_cache[0] = 2 */
    if (prime_count == 0) {
        prime_cache[prime_count++] = (seq_t)2;
        last_checked = 2;
    }

    /* Generate primes until we reach needed_index */
    for (; prime_count < needed_index;) {
        seq_t cand = last_checked + 1;
        /* Skip even numbers >2 */
        if ((cand & 1) == 0) cand++;

        /* Test divisibility by known primes up to sqrt(cand). */
        seq_t is_prime = 1;
        for (idx_t j = 1; j < prime_count; ++j) {   /* start at 3 */
            seq_t p = prime_cache[j];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            prime_cache[prime_count++] = cand;
        }
        last_checked = cand;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  If n == 0 return 0. */
    if (n == 0) return (seq_t)0;

    /* Need the primes at indices 2*n and 2*n+1 (1‑based). */
    idx_t needed = 2 * n + 1;
    ensure_primes(needed);

    /* Array indices are 0‑based; convert accordingly. */
    size_t idx_low  = (size_t)(2 * n - 1); /* prime(2n)   */
    size_t idx_high = (size_t)(2 * n);     /* prime(2n+1) */

    seq_t low  = prime_cache[idx_low];
    seq_t high = prime_cache[idx_high];

    return high - low;
}
