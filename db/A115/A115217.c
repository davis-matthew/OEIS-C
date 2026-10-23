/*
 * A115217  Diagonal sums of "correlation triangle" for 2^n.
 * https://oeis.org/A115217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)-3*a(n-3)-2*a(n-4)-2*a(n-5)+4*a(n-6)
 *            with initial values a(0)=1, a(1)=2, a(2)=6, a(3)=13, a(4)=30,
 *            a(5)=62
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"   /* seq_t *seq_alloc(size_t)                */
#include "../utils/seq_realloc.h"/* seq_t *seq_realloc(seq_t*,size_t,size_t)*/

static const seq_t base_terms[6] = {1, 2, 6, 13, 30, 62};

static seq_t *cache = NULL;     /* array of computed terms          */
static size_t cache_len = 0;    /* number of valid entries          */
static size_t cache_cap = 0;    /* current capacity of the cache    */

seq_t a(idx_t n)
{
    /* return pre‑computed fixed slice immediately */
    if (n < 6) return base_terms[n];

    /* grow cache if necessary */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 6;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
        if (!new_cache) exit(EXIT_FAILURE);   /* OOM */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise base terms if they are not yet stored */
    if (cache_len < 6) {
        for (size_t i = cache_len; i < 6; ++i) cache[i] = base_terms[i];
        cache_len = 6;
    }

    /* compute new terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t1 = cache[i-1] * 2;         /* 2*a(i-1) */
        seq_t t2 = cache[i-2] * 2;         /* 2*a(i-2) */
        seq_t t3 = cache[i-3] * 3;         /* 3*a(i-3) */
        seq_t t4 = cache[i-4] * 2;         /* 2*a(i-4) */
        seq_t t5 = cache[i-5] * 2;         /* 2*a(i-5) */
        seq_t t6 = cache[i-6] * 4;         /* 4*a(i-6) */

        cache[i] = t1 + t2 - t3 - t4 - t5 + t6;
    }
    cache_len = n + 1;
    return cache[n];
}
