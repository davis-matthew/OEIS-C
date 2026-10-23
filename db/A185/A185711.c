/*
 * A185711  a(n) = number of primes <= n that end in 1.
 * https://oeis.org/A185711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of primes ≤ n that end in the digit 1
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*               (i.e. prime p such that p % 10 == 1)                     */

#include <stdbool.h>
#include <stdlib.h>

/* Helper to build cumulative array of counts up to a given limit. */
static void build_cache(idx_t limit, seq_t **cache, idx_t *cap)
{
    /* Allocate new cache array (indices start at 0, but we ignore 0). */
    seq_t *new_cache = (seq_t*)calloc(limit + 1, sizeof(seq_t));
    if (*cache) free(*cache);
    *cache = new_cache;

    /* Basic sieve of Eratosthenes up to 'limit'. */
    bool *prime_flag = (bool*)malloc((limit + 1) * sizeof(bool));
    for (idx_t i = 0; i <= limit; ++i) prime_flag[i] = true;
    if (limit >= 0) prime_flag[0] = false;
    if (limit >= 1) prime_flag[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_flag[j] = false;
        }
    }

    /* Build cumulative counts of primes ending with 1. */
    seq_t count = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (prime_flag[i] && (i % 10) == 1) count++;
        new_cache[i] = count;
    }

    free(prime_flag);
    *cap = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache that persists across calls. */
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    if (n == 0) return 0;                 /* sequence is defined from n=1   */
    if (!cache || n > cap)                /* grow cache if needed          */
        build_cache(n, &cache, &cap);

    return cache[n];
}
