/*
 * A077706  a(1) = 7; a(n) is the smallest multiple of a(n-1) that contains
 *          all the digits of a(n-1) and is not a multiple of 10.
 * https://oeis.org/A077706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A077706: smallest multiple of the previous term that contains
 *  all *occurrences* of the digits of the previous term and is not
 *  a multiple of 10.
 *  a(1) = 7
 *  a(n) = min{ k·a(n-1) | k≥2, last digit of k·a(n-1)≠0,
 *              for every digit d, count_d(k·a(n-1)) ≥ count_d(a(n-1)) }
 *
 *  The function stores computed terms in a static cache so that calls
 *  in any order are fast.  It uses only the integer operators allowed
 *  by the judging environment, and works for all supported seq_t types
 *  (uint8_t … unsigned __int128 or a big‑integer class).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* count decimal digits of x into an array of 10 unsigned integers */
static void digit_counts(seq_t x, uint32_t *cnt)
{
    for (int d = 0; d < 10; ++d) cnt[d] = 0;
    while (x != (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        ++cnt[d];
        x /= (seq_t)10;
    }
}

/* cache of already computed terms (0‑based index) */
static seq_t *cache      = NULL;   /* array of a(n) values */
static idx_t  cached_size = 0;     /* number of stored terms   */
static idx_t  cache_cap   = 0;     /* allocated capacity */

/* enlarge cache to store at least 'min_cap' terms */
static void grow_cache(idx_t min_cap)
{
    if (cache_cap >= min_cap) return;
    idx_t new_cap = cache_cap ? (cache_cap << 1) : 8;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *new_cache = alloc_seq_array(new_cap);
    for (idx_t i = 0; i < cached_size; ++i) new_cache[i] = cache[i];
    if (cache) free_seq_array(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined below offset */

    /* ensure cache big enough */
    grow_cache(n);

    /* if already computed, return it */
    if (n <= cached_size) return cache[n-1];

    /* initialise first value if cache is empty */
    if (cached_size == 0) {
        cache[0] = (seq_t)7;               /* a(1) */
        cached_size = 1;
    }

    /* generate missing terms up to n */
    for (idx_t i = cached_size; i < n; ++i) {
        seq_t prev = cache[i-1];

        /* digit counts of previous term */
        uint32_t cnt_prev[10];
        digit_counts(prev, cnt_prev);

        /* iterate multiples of prev */
        seq_t inc = prev;
        seq_t cand = prev * (seq_t)2;      /* start with k = 2 */

        for (;;) {
            /* skip multiples ending with 0 */
            if ((cand % (seq_t)10) != (seq_t)0) {
                uint32_t cnt_cand[10];
                digit_counts(cand, cnt_cand);

                /* check all digit multiplicities */
                bool ok = true;
                for (int d = 0; d < 10; ++d) {
                    if (cnt_cand[d] < cnt_prev[d]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) break;            /* found the next term */
            }
            cand += inc;                  /* next multiple */
        }

        cache[i] = cand;                  /* store a(i+1) */
    }

    cached_size = n;
    return cache[n-1];
}
