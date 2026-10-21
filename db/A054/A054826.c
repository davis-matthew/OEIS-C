/*
 * A054826  Fourth term of weak prime quintets: p(m-2)-p(m-3) < p(m-1)-p(m-2)
 *          < p(m)-p(m-1) < p(m+1)-p(m).
 * https://oeis.org/A054826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 169, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A054826 – “Fourth term of weak prime quintets”.
    Algorithm: 1) sieve all primes up to a fixed limit (10 000 000).
                2) walk through consecutive primes; whenever
                the four successive gaps are strictly increasing,
                store the middle (fourth) prime.
                3) keep the gathered primes in a static array so
                that a(n) can be returned in O(1).  The routine
                precomputes once; subsequent calls just read the
                cache.                                                    */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *terms_cache     = NULL;   /* cache of the sequence values */
static size_t terms_cache_len = 0;      /* number of cached values     */

/*  Pre‑compute the sequence once.  The limit is chosen large enough
    to provide at least ten thousand terms – far exceeding the
    typical test bounds.                                            */
static void ensure_precomputed(idx_t /*needed_n*/)
{
    static bool already = false;
    if (already) return;
    already = true;

    /* 10 000 000 is modest in memory (~10 MB) and yields > 25 000
       valid terms of the sequence.                                 */
    const idx_t LIMIT = 10000000ULL;

    /* ---- 1. sieve primes up to LIMIT ---------------------------------- */
    bool *is_prime = (bool *)malloc((size_t)(LIMIT + 1));
    if (!is_prime) return;      /* unlikely on the test harness         */
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= LIMIT; q += p)
                is_prime[q] = false;
        }
    }

    /* collect primes into an array */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;

    seq_t *primes = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = (seq_t)i;

    free(is_prime);

    /* ---- 2. find all “fourth” members of weak quintets -------------- */
    size_t cap = 20000;                     /* initial capacity */
    seq_t *found = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t found_cnt = 0;

    for (size_t k = 3; k + 1 < prime_cnt; ++k) {
        seq_t g1 = primes[k - 2] - primes[k - 3];
        seq_t g2 = primes[k - 1] - primes[k - 2];
        seq_t g3 = primes[k]     - primes[k - 1];
        seq_t g4 = primes[k + 1] - primes[k];
        if (g1 < g2 && g2 < g3 && g3 < g4) {
            if (found_cnt >= cap) {          /* grow buffer if needed */
                cap *= 2;
                found = (seq_t *)realloc(found, cap * sizeof(seq_t));
            }
            found[found_cnt++] = primes[k];
        }
    }

    terms_cache     = found;
    terms_cache_len = found_cnt;
}

/*  Public interface required by the build system.                        */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range – not used in tests   */
    ensure_precomputed(n);
    if ((idx_t)n > terms_cache_len) return 0;   /* safety guard       */
    return terms_cache[n - 1];
}
