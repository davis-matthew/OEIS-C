/*
 * A185715  a(n) = number of primes <= n that end in 9.
 * https://oeis.org/A185715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A185715: a(n) = number of primes ≤ n that end in 9.
   The function uses a simple Eratosthenes sieve and keeps a static cache
   so that each n is computed at most once. */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of cumulative counts */
    static seq_t *counts = NULL;
    static idx_t current_limit = 0;

    /* sequences are defined for n ≥ 1 */
    if (n == 0)
        return 0;

    /* if we already have data up to this n, just return it */
    if (n <= current_limit)
        return counts[(size_t)n];

    /* allocate a new cache big enough for the largest requested n */
    idx_t new_limit = n;
    seq_t *new_counts = alloc_seq_array((size_t)new_limit + 1);
    if (!new_counts)
        return 0;               /* allocation failed */

    /* sieve of Eratosthenes up to new_limit */
    bool *prime = (bool *)malloc((size_t)(new_limit + 1) * sizeof(bool));
    if (!prime) {
        free_seq_array(new_counts);
        return 0;
    }
    for (size_t i = 0; i <= new_limit; ++i)
        prime[i] = true;
    if (new_limit >= 0)
        prime[0] = false;
    if (new_limit >= 1)
        prime[1] = false;

    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (prime[(size_t)i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                prime[(size_t)j] = false;
        }
    }

    /* compute cumulative counts of primes ending in 9 */
    seq_t cum = 0;
    for (idx_t i = 0; i <= new_limit; ++i) {
        if (prime[(size_t)i] && (i % 10 == 9))
            cum += (seq_t)1;
        new_counts[(size_t)i] = cum;
    }

    free(prime);

    /* replace old cache (if any) */
    if (counts)
        free_seq_array(counts);
    counts = new_counts;
    current_limit = new_limit;

    return counts[(size_t)n];
}
