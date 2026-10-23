/*
 * A015551  Expansion of x/(1 - 6*x - 5*x^2).
 * https://oeis.org/A015551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 5*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache   = NULL;
static size_t seq_cap     = 0;
static size_t seq_len     = 0;

seq_t a(idx_t n)
{
    size_t ns = (size_t)n;

    /* Initialize cache on first call */
    if (seq_cache == NULL) {
        seq_cap   = 2;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_cache[0] = (seq_t)0;
        seq_cache[1] = (seq_t)1;
        seq_len = 2;
    }

    if (ns < seq_len)
        return seq_cache[ns];

    /* Grow capacity to at least ns+1 */
    if (ns + 1 > seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap < ns + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1); /* allocation failed */
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = seq_len; i <= ns; ++i) {
        seq_cache[i] =
            (seq_t)6 * seq_cache[i - 1] + (seq_t)5 * seq_cache[i - 2];
    }
    seq_len = ns + 1;

    return seq_cache[ns];
}
