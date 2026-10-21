/*
 * A094911  Expansion of e.g.f. exp(7*x)/(1-7*x)^(1/7).
 * https://oeis.org/A094911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=8, for n≥2 a(n) = (7*n+1)*a(n-1) - 49*(n-1)*a(n-2)
 *            (recurrence from the OEIS entry)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *seq_cache = NULL;     /* array holding computed terms */
static idx_t seq_cache_len = 0;     /* current number of stored terms */

/* Expand the cache to contain terms up to index n */
static void ensure_cache(idx_t n)
{
    if (n + 1U <= seq_cache_len)
        return;   /* already computed */

    /* Reallocate to the required size */
    seq_t *tmp = (seq_t *)realloc(seq_cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp)
        exit(1);  /* allocation failure */
    seq_cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t i = seq_cache_len; i <= n; ++i) {
        if (i == 0) {
            seq_cache[0] = (seq_t)1;
        } else if (i == 1) {
            seq_cache[1] = (seq_t)8;
        } else {
            /* Use mul_small to avoid overflow of the intermediate product */
            seq_t part1 = mul_small(seq_cache[i - 1], (unsigned int)(7 * i + 1));
            seq_t part2 = mul_small(seq_cache[i - 2], (unsigned int)(49 * (i - 1)));
            seq_cache[i] = part1 - part2;
        }
    }
    seq_cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
