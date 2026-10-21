/*
 * A072110  a(n) = 4*a(n-1) - a(n-2) - 2, with a(0)=1, a(1)=2.
 * https://oeis.org/A072110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 2, a(0)=1, a(1)=2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>              /* for realloc */

static seq_t *cache = NULL;   /* cache of computed terms */
static idx_t cache_len = 0;   /* number of entries currently cached (index+1) */

/* Ensure that a(0) … a(n) are stored in cache */
static void ensure_up_to(idx_t n)
{
    /* already have everything */
    if (cache_len > n)
        return;

    /* make sure we have at least two base terms */
    if (cache_len == 0) {
        cache = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
    } else if (cache_len == 1) {
        seq_t *tmp = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        cache = tmp;
        /* base case a(1) is missing; set it */
        cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* enlarge storage if necessary */
    if (cache_len <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = tmp;
    }

    /* compute all missing terms */
    seq_t a_prev2 = cache[cache_len - 2];
    seq_t a_prev1 = cache[cache_len - 1];
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t cur = (seq_t)4 * a_prev1 - a_prev2 - (seq_t)2;
        cache[i] = cur;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }

    cache_len = n + 1;
}

/* External function required by the harness */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n];
}
