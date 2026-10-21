/*
 * A203524  a(n) = Product_{2 <= i < j <= n+1} (prime(i) + prime(j)).
 * https://oeis.org/A203524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{2 <= i < j <= n+1} (prime(i) + prime(j))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Types seq_t and idx_t are defined by the build system. */

/* --------------------------------------------------------------------- */
/* Prime generation and caching                                          */
/* --------------------------------------------------------------------- */
static uint64_t *primes = NULL;        /* 0‑based: primes[0] = 2          */
static size_t   primes_cnt = 0;        /* number of primes stored         */

/* Ensure at least 'needed_idx' primes are generated (prime indices start at 1). */
static void ensure_primes(idx_t needed_idx)
{
    while (primes_cnt < (size_t)needed_idx) {
        uint64_t cand = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
        for (;;) {
            bool ok = true;
            for (size_t pi = 0; pi < primes_cnt && (uint64_t)primes[pi] * primes[pi] <= cand; ++pi) {
                if (cand % primes[pi] == 0) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                uint64_t *tmp = (uint64_t*)realloc(primes,
                                                   (primes_cnt + 1) * sizeof(uint64_t));
                if (!tmp) abort();   /* abort on allocation failure */
                primes = tmp;
                primes[primes_cnt++] = cand;
                break;
            }
            ++cand;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Sequence computation                                                 */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;          /* cache[n-1] = a(n)               */
static size_t cache_cap = 0;          /* allocated size of cache         */
static idx_t  max_cached = 0;        /* largest n whose a(n) is cached  */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined but safe */
    }

    /* Require primes up to index n+1 (since j <= n+1) */
    ensure_primes(n + 1);

    /* Ensure cache array has space */
    if ((size_t)n > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = n;
    }

    /* Return cached value if already computed */
    if (n <= max_cached) {
        return cache[n - 1];
    }

    /* Initialize with the last cached value (or 1 if none) */
    seq_t cur = (max_cached == 0) ? (seq_t)1 : cache[max_cached - 1];

    /* Compute new terms incrementally */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        seq_t prod = (seq_t)1;
        uint64_t pk = primes[k];   /* prime(k+1) */
        for (idx_t i = 2; i <= k; ++i) {
            uint64_t pi = primes[i - 1];   /* prime(i) */
            prod *= (seq_t)(pi + pk);
        }
        cur *= prod;
        cache[k - 1] = cur;
    }

    max_cached = n;
    return cache[n - 1];
}
