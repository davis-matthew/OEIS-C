/*
 * A175444  a(n)>a(n-1), a(n) = smallest prime such that a(n)+a(n-1) is
 *          multiple of m, a(1)=2, m=7.
 * https://oeis.org/A175444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 991, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175444:  a(1)=2; for n>1
   a(n) = the smallest prime p > a(n-1) such that (p + a(n-1)) % 7 == 0
   (m = 7) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache      = NULL;   /* memoised terms */
static idx_t cache_size   = 0;     /* number of stored terms */
static size_t cache_cap   = 0;     /* allocated capacity */

/* Return the n‑th term (n≥1).  The function is memoised so any order of
   calls is handled in O(n) amortised time. */
seq_t a(idx_t n)
{
    while (cache_size < n) {
        /* grow the memoisation array if necessary */
        if (cache_size == cache_cap) {
            size_t newcap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
            if (cache) {
                /* copy the existing values to the new buffer */
                for (idx_t i = 0; i < cache_size; ++i)
                    tmp[i] = cache[i];
                free(cache);
            }
            cache      = tmp;
            cache_cap  = newcap;
        }

        /* initialise the first term */
        if (cache_size == 0) {
            cache[0] = (seq_t)2;
            cache_size = 1;
            continue;
        }

        /* compute the next term */
        seq_t prev = cache[cache_size - 1];
        uint64_t prev64 = (uint64_t)prev;
        uint64_t cand = prev64 + 1;

        for (;;) {
            if (is_prime_big((seq_t)cand) &&
                ((cand + prev64) % 7 == 0)) {
                cache[cache_size] = (seq_t)cand;
                break;
            }
            ++cand;
        }
        ++cache_size;
    }

    /* return the requested term (sequence is 1‑based) */
    return cache[n - 1];
}
