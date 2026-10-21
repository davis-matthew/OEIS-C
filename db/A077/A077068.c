/*
 * A077068  Semiprimes of the form prime + 1.
 * https://oeis.org/A077068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = semiprime that is one more than a prime (i.e., n = p*q
 *            with p prime, q prime, and n-1 prime)
 * Range:     u8 n <= 8, u16 n <= 486, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/count_prime_factors.h"

/* Cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;       /* number of terms stored */
static size_t cache_cap = 0;       /* allocated capacity of cache */
static idx_t next_candidate = 4;   /* next integer to test */

/* Ensure the cache can hold at least 'needed' terms. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);          /* simple failure handling */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term (n >= 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n==0; placeholder */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Find the next candidate that satisfies the conditions. */
        while (1) {
            idx_t cand = next_candidate;
            /* Candidate must be semiprime and cand-1 must be prime. */
            if (is_prime(cand - 1) && count_prime_factors((seq_t)cand) == 2) {
                /* Store the term. */
                grow_cache(cache_len + 1);
                cache[cache_len++] = (seq_t)cand;
                break;
            }
            ++next_candidate;
        }
        /* Advance candidate for the next search. */
        ++next_candidate;
    }

    /* Sequence is 1‑based. */
    return cache[n - 1];
}
