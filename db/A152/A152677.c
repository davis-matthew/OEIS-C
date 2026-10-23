/*
 * A152677  Subsequence of odd terms in A000203 (sum-of-divisors function
 *          sigma), in the order in which they occur and with repetitions.
 * https://oeis.org/A152677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(A028982(n))
 * Range:     u8 n <= 19, u16 n <= 252, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A028982(n) is the n‑th integer that is either a perfect square or twice a perfect square */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"

/* Static cache for already computed terms */
static seq_t* cache = NULL;     /* array of computed a(i) values */
static size_t cache_len = 0;    /* number of values stored */
static size_t cache_cap = 0;    /* allocated capacity */

/* Ensure the cache can hold at least 'need' entries */
static void ensure_cache(size_t need)
{
    if (cache_len >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t* tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a new value to the cache */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t* tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Compute the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined offset, but harmless */

    /* Compute terms up to n if needed */
    while (cache_len < n) {
        /* Static state to generate A028982 in order */
        static idx_t sq_idx = 1;            /* current k for k^2          */
        static idx_t tsq_idx = 1;           /* current k for 2*k^2        */
        static idx_t next_sq = 1;           /* sq_idx^2                   */
        static idx_t next_tsq = 2;          /* 2 * tsq_idx^2              */

        idx_t m;                            /* next index in A028982      */
        if (next_sq <= next_tsq) {
            m = next_sq;
            sq_idx++;
            next_sq = sq_idx * sq_idx;
        } else {
            m = next_tsq;
            tsq_idx++;
            next_tsq = 2 * tsq_idx * tsq_idx;
        }

        seq_t val = sigma_of(m);            /* compute sigma(m)           */
        append_cache(val);                  /* store result               */
    }

    return cache[n - 1];
}
