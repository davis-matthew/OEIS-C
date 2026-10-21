/*
 * A071626  Number of distinct exponents in the prime factorization of n!.
 * https://oeis.org/A071626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct exponents appearing in the * prime
 *            factorization of n! (equivalently the number of distinct values
 *            * floor(n/p) for primes p ≤ n)
 * Range:     u8 n <= 3541, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* The build supplies the types idx_t (typically uint64_t) and seq_t
 * (unsigned of suitable width).  Do not redeclare them. */

/* ---------- static data ---------- */
static seq_t *seq_cache    = NULL;    /* cache of computed a(n), index 1‑based */
static size_t  cache_size  = 0;      /* allocated length of seq_cache  */

static idx_t *primes      = NULL;    /* list of primes up to current limit */
static size_t  primes_cnt  = 0;      /* number of primes stored          */
static idx_t   primes_lim  = 0;      /* largest number for which primes are known */

/* ---------- build primes up to limit ---------- */
static void build_primes_up_to(idx_t limit)
{
    if (limit <= primes_lim) return;              /* already done */

    /* free old list if any */
    free(primes);
    primes = NULL;
    primes_cnt = 0;
    primes_lim = limit;

    /* simple sieve */
    size_t sz = (size_t)(limit + 1);
    uint8_t *sieve = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!sieve) exit(EXIT_FAILURE);

    /* count primes */
    for (idx_t p = 2; p <= limit; ++p)
        if (!sieve[p]) {
            if (p <= limit / p)
                for (idx_t m = p * p; m <= limit; m += p)
                    sieve[m] = 1;
        }

    /* allocate array for primes */
    size_t cnt = 0;
    for (idx_t p = 2; p <= limit; ++p)
        if (!sieve[p]) ++cnt;

    primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!primes) exit(EXIT_FAILURE);

    size_t pos = 0;
    for (idx_t p = 2; p <= limit; ++p)
        if (!sieve[p]) primes[pos++] = p;

    primes_cnt = cnt;
    free(sieve);
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined below 1, return 0 */

    /* extend cache if necessary */
    if ((size_t)n >= cache_size) {
        /* recompute primes up to n once */
        build_primes_up_to(n);

        size_t new_size = (size_t)n + 1;
        seq_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!seq_cache) exit(EXIT_FAILURE);

        /* compute missing entries */
        for (size_t i = cache_size; i <= n; ++i) {
            idx_t cur = (idx_t)i;
            /* boolean array of seen exponents; size cur+1 is enough */
            uint8_t *seen = (uint8_t *)calloc(cur + 1, sizeof(uint8_t));
            if (!seen) exit(EXIT_FAILURE);
            seq_t distinct = 0;

            /* iterate all primes ≤ cur */
            for (size_t j = 0; j < primes_cnt && primes[j] <= cur; ++j) {
                idx_t p = primes[j];
                seq_t e = 0;
                idx_t power = p;
                while (power <= cur) {
                    e += (seq_t)(cur / power);
                    /* avoid overflow for next multiplication */
                    if (power > cur / p) break;
                    power *= p;
                }
                if (!seen[(size_t)e]) {
                    seen[(size_t)e] = 1;
                    ++distinct;
                }
            }
            free(seen);
            seq_cache[i] = distinct;
        }
        cache_size = new_size;
    }

    return seq_cache[n];
}
