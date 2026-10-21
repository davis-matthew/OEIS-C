/*
 * A087188  Number of partitions of n into distinct squarefree parts.
 * https://oeis.org/A087188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct squarefree parts
 * Range:     u8 n <= 43, u16 n <= 121, u32 n <= 377, u64 n <= 1277,
 *            u128 n <= 4592, big any n
 * Helpers:   alloc_memory, free_memory
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/alloc_memory.h"
#include "../utils/free_memory.h"

/* ---------- DP cache ---------- */
static seq_t *dp_cache = NULL;   /* DP array */
static idx_t dp_cached = 0;      /* size of dp_cache (number of entries) */

/* ---------- recompute DP up to `limit` ---------- */
static void compute_dp(idx_t limit) {
    idx_t size = limit + 1;                        /* indices 0 .. limit */

    /* Allocate DP array */
    seq_t *dp = (seq_t *)alloc_memory(size * sizeof(seq_t));
    /* Initialize to zero */
    {
        idx_t i;
        for (i = 0; i < size; ++i) dp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;     /* empty partition */

    /* Sieve for primes up to limit */
    bool *is_prime = (bool *)alloc_memory(size * sizeof(bool));
    {
        idx_t i;
        for (i = 0; i < size; ++i) is_prime[i] = true;
    }
    if (size > 0) is_prime[0] = false;
    if (size > 1) is_prime[1] = false;
    for (idx_t i = 2; i * i < size; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j < size; j += i) is_prime[j] = false;
        }
    }

    /* Mark squarefree numbers */
    bool *sqfree = (bool *)alloc_memory(size * sizeof(bool));
    {
        idx_t i;
        for (i = 0; i < size; ++i) sqfree[i] = true;
    }
    if (size > 0) sqfree[0] = false;
    for (idx_t p = 2; p < size; ++p) if (is_prime[p]) {
        for (idx_t j = p * p; j < size; j += p * p) sqfree[j] = false;
    }

    /* DP over all squarefree numbers */
    for (idx_t s = 1; s < size; ++s) {
        if (!sqfree[s]) continue;
        for (idx_t sum = limit; sum >= s; --sum) {
            dp[sum] += dp[sum - s];
        }
    }

    /* Free temporary arrays */
    free_memory(is_prime);
    free_memory(sqfree);

    /* Replace old cache */
    if (dp_cache) free_memory(dp_cache);
    dp_cache  = dp;
    dp_cached = size;
}

/* ---------- required function ---------- */
seq_t a(idx_t n) {
    if (n + 1 > dp_cached)            /* need to recompute up to new n */
        compute_dp(n);
    return dp_cache[n];
}
