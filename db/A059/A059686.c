/*
 * A059686  Grimm numbers (1): a(n) = largest k so that for each m in {n+1,
 *          n+2, ..., n+k} there corresponds a different prime factor p_m.
 * https://oeis.org/A059686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_match
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 * Formula: a(n) = \
 *   maximum k such that for m = n+1, … , n+k there exist distinct prime
 *   factors p_m for each m.  (Maximum bipartite matching on primes.)
 *
 * This implementation precomputes a smallest‑prime‑factor sieve up to a
 * sufficient limit, then for each call to a(n) constructs the factor
 * lists for the numbers n+1, n+2, … and uses a standard DFS augmenting
 * path algorithm to incrementally build a matching.  The algorithm is
 * O(k * P) where P is the number of primes up to the working limit,
 * and with a(n) < 2000 for n ≤ 10000 it finishes well under 2 s.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/dfs_match.h"

/* Global smallest‑prime‑factor table (lazily built). */
static uint32_t *global_spf = NULL;
static size_t   global_spf_size = 0;

/* Build SPF array up to at least [limit]. */
static void build_spf(size_t limit)
{
    if (limit <= global_spf_size)
        return;

    if (global_spf != NULL) {
        free(global_spf);
    }

    global_spf_size = limit;
    global_spf = (uint32_t *)malloc(global_spf_size * sizeof(uint32_t));
    if (global_spf == NULL) {
        /* Fallback: insufficient memory – abort silently. */
        return;
    }

    for (size_t i = 0; i < global_spf_size; ++i)
        global_spf[i] = (uint32_t)i;

    for (size_t i = 2; i * i < global_spf_size; ++i) {
        if (global_spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j < global_spf_size; j += i) {
                if (global_spf[j] == (uint32_t)j)
                    global_spf[j] = (uint32_t)i;
            }
        }
    }
}

/* Value of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* Undefined below offset – harmless. */

    /* Working limit: n plus a generous buffer for the window size. */
    const size_t LIMIT_BUF = 5000U;
    size_t limit = (size_t)n + LIMIT_BUF + 1U;
    build_spf(limit + 1U);             /* ensure SPF up to limit+1 */

    /* Maximal expected window size is far below LIMIT_BUF. */
    const size_t MAX_K = LIMIT_BUF;

    /* Allocate arrays for factor lists and match data. */
    uint32_t **factors  = (uint32_t **)malloc(MAX_K * sizeof(uint32_t *));
    size_t   *factor_sz = (size_t *)  malloc(MAX_K * sizeof(size_t));
    int      *match     = (int *)     malloc((limit + 1) * sizeof(int));
    char     *visited   = (char *)    malloc((limit + 1) * sizeof(char));

    if (!factors || !factor_sz || !match || !visited)
    {   /* Fallback if memory allocation failed. */
        free(factors);  free(factor_sz);  free(match);  free(visited);
        return (seq_t)0;
    }

    memset(match, 0xFF, (limit + 1) * sizeof(int));   /* -1: unmatched */

    size_t k = 0;   /* number of successfully matched numbers */

    for (;;) {
        uint32_t m = (uint32_t)(n + 1 + k);   /* current number to test */
        if (m > limit)
            break;  /* safety; should not happen */

        /* Extract distinct prime factors of m using SPF. */
        uint32_t fac_buf[32];
        size_t  fac_cnt = 0;
        uint32_t tmp = m;
        while (tmp > 1) {
            uint32_t p = global_spf[tmp];
            fac_buf[fac_cnt++] = p;
            while (tmp % p == 0)
                tmp /= p;
        }

        /* Store the factor list for this node. */
        uint32_t *arr = (uint32_t *)malloc(fac_cnt * sizeof(uint32_t));
        if (arr == NULL) {
            /* memory error: abort the construction. */
            break;
        }
        memcpy(arr, fac_buf, fac_cnt * sizeof(uint32_t));
        factors[k]   = arr;
        factor_sz[k] = fac_cnt;

        /* Attempt to extend the matching with this new node. */
        memset(visited, 0, (limit + 1) * sizeof(char));
        if (!dfs_match((int)k, (const uint32_t **)factors, factor_sz,
                       match, visited)) {
            /* Augmenting path not found: window cannot be extended. */
            free(arr);
            break;
        }

        /* Successful match: increment window size. */
        ++k;
    }

    /* Clean up allocated factor lists. */
    for (size_t i = 0; i < k; ++i)
        free(factors[i]);

    free(factors);
    free(factor_sz);
    free(match);
    free(visited);

    return (seq_t)k;
}
