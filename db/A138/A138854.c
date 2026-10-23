/*
 * A138854  Numbers which are the sum of three cubes of distinct primes.
 * https://oeis.org/A138854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A138854: Numbers which are the sum of three cubes of distinct primes
    (OEIS offset 1).  The function returns the nth term (n ≥ 1).  It
    enumerates all unordered triples of distinct primes, computes their
    cube sums, sorts, deduplicates, and caches the results.  The cache is
    expanded as needed so that the function works for any call order. */

/*  build_primes(limit, out, cnt)  is a helper from ../utils/sieve_primes.h.
    It creates an array of primes <= limit and writes the pointer to *out
    and count to *cnt.  The caller must free the array. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  Comparator for qsort using seq_t. */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/*  Generate all primes <= limit.
    The array is allocated with malloc and must be freed by the caller. */
static void generate_primes(idx_t limit, uint32_t **out, size_t *cnt)
{
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    for (idx_t i = 2; i <= limit; ++i) is_prime[(size_t)i] = true;
    for (idx_t p = 2; (idx_t)p * p <= limit; ++p)
        if (is_prime[(size_t)p])
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[(size_t)m] = false;

    size_t npr = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i]) ++npr;

    uint32_t *pr = (uint32_t *)malloc(npr * sizeof(uint32_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i]) pr[pos++] = (uint32_t)i;

    free(is_prime);
    *out = pr;
    *cnt = npr;
}

/*  Static cache state.  The cache grows only when more terms are required. */
static seq_t       *sums_cache = NULL;     /* sorted, deduped sums          */
static size_t      sums_len   = 0;          /* number of stored sums          */
static uint32_t    *primes     = NULL;      /* list of primes up to prime_lim */
static size_t      prime_cnt  = 0;          /* number of primes in the list   */
static idx_t       prime_lim  = 0;          /* current upper bound for primes */

static void ensure_sums(idx_t n)
{
    if (n == 0) return;                     /* nothing required for n==0     */
    if (sums_len >= (size_t)n) return;      /* already have enough terms     */

    /* Expand the cache until we have at least n terms. */
    while (sums_len < (size_t)n)
    {
        /* Choose a bound for primes: double until good enough. */
        idx_t new_limit = (prime_lim == 0) ? 50 : prime_lim * 2;

        /* Free old prime list and build a new one. */
        if (primes) free(primes);
        generate_primes(new_limit, &primes, &prime_cnt);
        prime_lim = new_limit;

        /* Number of unordered triples of distinct primes. */
        size_t tot = 0;
        if (prime_cnt >= 3)
            tot = prime_cnt * (prime_cnt - 1) * (prime_cnt - 2) / 6;

        /* Temporary array for all sums. */
        seq_t *tmp = (seq_t *)malloc(tot * sizeof(seq_t));

        /* Compute all cube sums. */
        size_t idx = 0;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            seq_t pi = (seq_t)primes[i];
            seq_t pi3 = pi * pi * pi;
            for (size_t j = 0; j < i; ++j)
            {
                seq_t pj = (seq_t)primes[j];
                seq_t pj3 = pj * pj * pj;
                for (size_t k = 0; k < j; ++k)
                {
                    seq_t pk = (seq_t)primes[k];
                    seq_t pk3 = pk * pk * pk;
                    tmp[idx++] = pi3 + pj3 + pk3;
                }
            }
        }

        /* Sort the sums. */
        qsort(tmp, tot, sizeof(seq_t), seq_cmp);

        /* Deduplicate into the cache array. */
        if (sums_cache) free(sums_cache);
        sums_cache = (seq_t *)calloc(tot, sizeof(seq_t));

        size_t uniq = 0;
        if (tot > 0)
        {
            sums_cache[0] = tmp[0];
            uniq = 1;
            for (size_t i = 1; i < tot; ++i)
                if (tmp[i] != tmp[i-1])
                    sums_cache[uniq++] = tmp[i];
        }
        free(tmp);

        sums_len = uniq;
        /* Loop again if still not enough terms. */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */
    ensure_sums(n);                      /* guarantee cache is large enough */
    return sums_cache[(size_t)(n - 1)];
}
