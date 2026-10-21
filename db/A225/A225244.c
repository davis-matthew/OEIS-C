/*
 * A225244  Number of partitions of n into squarefree divisors of n.
 * https://oeis.org/A225244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 89, u32 n <= 329, u64 n <= 1889,
 *            u128 n <= 9239, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225244: number of partitions of n into square‑free divisors of n */
#include <stdlib.h>
#include <stdint.h>

/* Memoisation cache for already computed terms */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* Grow the cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* allocation failure */
        for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return memoised value if present */
    if (cache[n] != (seq_t)0) return cache[n];

    /* Step 1: factorise n to obtain distinct prime factors */
    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    int pcnt = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            primes[pcnt++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) primes[pcnt++] = m;

    /* Step 2: generate all square‑free divisors of n */
    size_t divs_cap = 1;
    idx_t *divs = (idx_t *)malloc(divs_cap * sizeof(idx_t));
    if (!divs) return (seq_t)0;           /* allocation failure */
    divs[0] = (idx_t)1;
    size_t divs_cnt = 1;
    for (int i = 0; i < pcnt; ++i) {
        idx_t ip = (idx_t)primes[i];
        size_t cur = divs_cnt;
        for (size_t j = 0; j < cur; ++j) {
            idx_t val = divs[j] * ip;
            if (divs_cnt == divs_cap) {
                divs_cap *= 2;
                idx_t *tmp2 = (idx_t *)realloc(divs, divs_cap * sizeof(idx_t));
                if (!tmp2) {
                    free(divs);
                    return (seq_t)0;
                }
                divs = tmp2;
            }
            divs[divs_cnt++] = val;
        }
    }

    /* Step 3: dynamic programming for coin change with these divisors */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) {
        free(divs);
        return (seq_t)0;                   /* allocation failure */
    }
    dp[0] = (seq_t)1;
    for (size_t di = 0; di < divs_cnt; ++di) {
        idx_t d = divs[di];
        for (idx_t s = d; s <= n; ++s) {
            dp[s] += dp[s - d];
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(divs);

    /* Step 4: cache and return */
    cache[n] = result;
    return result;
}
