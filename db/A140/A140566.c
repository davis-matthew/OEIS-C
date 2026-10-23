/*
 * A140566  List of prime quartets (k - 11, k - 1, k + 1, k + 11).
 * https://oeis.org/A140566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the sequence formed by concatenating all prime *
 *            quadruples (k-11, k-1, k+1, k+11) for which all four numbers are
 *            * prime. Each quartet is appended in order k-11, k-1, k+1, k+11,
 *            * and quartets are processed in increasing k. The first quartet
 *            * occurs for k = 18 giving (7,17,19,29). The se...
 * Range:     u8 n <= 24, u16 n <= 419, u32 n <= 207, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;   /* cached terms of the sequence */
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 18;      /* next value of k to test (first candidate) */

static inline void grow_cache(size_t needed)
{
    if (cache_len + needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < cache_len + needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

static inline void append_to_cache(seq_t val)
{
    grow_cache(1);
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    size_t target = (size_t)n;
    while (cache_len < target) {
        /* Search for the next k that yields a valid quartet */
        while (true) {
            if (is_prime_big((seq_t)(next_k - 11)) &&
                is_prime_big((seq_t)(next_k - 1))  &&
                is_prime_big((seq_t)(next_k + 1))  &&
                is_prime_big((seq_t)(next_k + 11)))
            {
                break;
            }
            next_k += 2;   /* k must be even, start from the first even k */
        }

        /* Append the quartet in the required order */
        append_to_cache((seq_t)(next_k - 11));
        append_to_cache((seq_t)(next_k - 1));
        append_to_cache((seq_t)(next_k + 1));
        append_to_cache((seq_t)(next_k + 11));

        /* Advance k for the next search */
        next_k += 2;
    }
    return cache[n - 1];
}
