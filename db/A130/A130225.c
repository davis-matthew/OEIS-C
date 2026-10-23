/*
 * A130225  a(1) = 2; a(n) = a(n-1) + (number of times the digit 2 has
 *          appeared in the sequence so far).
 * https://oeis.org/A130225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 518, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq, digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130225: a(1)=2, a(n)=a(n-1)+count of digit 2 seen in all previous terms */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"
#include "../utils/alloc_seq.h"

static seq_t *cache = NULL;        /* store a(1)..a(N) (0‑based) */
static size_t cache_len = 0;       /* number of values stored */
static size_t cache_cap = 0;       /* allocated capacity of cache */
static seq_t two_count = 0;        /* total number of digit '2' seen so far */

/* Ensure cache can hold at least 'need' entries, reallocating if necessary */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = alloc_seq(new_cap);
    if (!tmp)
        return;          /* allocation failed – silently abort */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, return 0 */

    /* Compute values up to index n (1‑based) */
    while (cache_len < (size_t)n) {
        if (cache_len == 0) {            /* first term a(1)=2 */
            ensure_cache(1);
            cache[0] = (seq_t)2;
            two_count = (seq_t)1;        /* one '2' in the term */
            ++cache_len;
            continue;
        }

        seq_t prev = cache[cache_len - 1];
        seq_t new_val = prev + two_count;

        unsigned char cnt[10];
        digit_counts(new_val, cnt);
        two_count += (seq_t)cnt[2];

        ensure_cache(cache_len + 1);
        cache[cache_len] = new_val;
        ++cache_len;
    }

    return cache[n - 1];   /* zero‑based index */
}
