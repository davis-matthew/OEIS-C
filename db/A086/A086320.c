/*
 * A086320  a(n) is the depth of the prime tree formed when 4p +- 3 is applied
 *          to the n-th prime and repeatedly to any primes generated from the
 *          n-th prime via this process.
 * https://oeis.org/A086320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 9909, u128 n <= 9909,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the depth of the prime tree defined by 4p ± 3 for the n‑th prime. */
/* The depth is the longest chain of primes reachable by repeatedly replacing a prime p with either 4p+3 or 4p−3 (if the result is prime). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

typedef struct {
    seq_t prime;  /* the prime number */
    seq_t depth;  /* depth of its tree */
} prime_depth_t;

/* Cache of already computed depths */
static prime_depth_t *cache_ptr = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;

/* Recursive helper that returns the depth of the prime tree rooted at p */
static seq_t depth_of_prime(seq_t p)
{
    /* Try to find a memoised result */
    for (size_t i = 0; i < cache_len; ++i) {
        if (cache_ptr[i].prime == p) {
            return cache_ptr[i].depth;
        }
    }

    /* Compute depths of children 4p+3 and 4p−3 if they are prime */
    seq_t max_child_depth = 0;

    seq_t child = p * 4 + 3;
    if (is_prime_big(child)) {
        seq_t d = depth_of_prime(child);
        if (d > max_child_depth) max_child_depth = d;
    }

    child = p * 4 - 3;
    if (is_prime_big(child)) {
        seq_t d = depth_of_prime(child);
        if (d > max_child_depth) max_child_depth = d;
    }

    /* Depth of current node is one more than the maximum child depth */
    seq_t depth = max_child_depth + 1;

    /* Store result in cache */
    size_t needed = cache_len + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < needed) new_cap = needed;
        prime_depth_t *new_cache = (prime_depth_t *)realloc(cache_ptr, new_cap * sizeof(prime_depth_t));
        if (!new_cache) abort();   /* out of memory, terminate */
        cache_ptr = new_cache;
        cache_cap = new_cap;
    }
    cache_ptr[cache_len].prime = p;
    cache_ptr[cache_len].depth = depth;
    ++cache_len;

    return depth;
}

/* Main sequence function: a(n) = depth of the prime tree rooted at the n-th prime */
seq_t a(idx_t n)
{
    /* get_prime(n) returns the n-th prime (1‑indexed) as a uint32_t */
    uint32_t p = get_prime(n);
    return depth_of_prime((seq_t)p);
}
