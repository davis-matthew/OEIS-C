/*
 * A063216  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          49 ).
 * https://oeis.org/A063216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) – a(n-5) Initial values (for n >= 1):
 *            a(1)=1, a(2)=8, a(3)=14, a(4)=22, a(5)=28
 * Range:     u8 n <= 38, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] unused, cache[1].. hold values */
static size_t cache_cap = 0;     /* allocated size (number of slots) */
static idx_t   max_computed = 0; /* largest index for which cache is filled */

/* Compute the n‑th term of OEIS A063216. */
seq_t a(idx_t n)
{
    if (n < 1)             /* sequence is defined only for n >= 1 */
        return 0;

    /* Allocate or grow the cache on the first use or when n exceeds capacity */
    if (cache == NULL) {
        cache_cap = (n > 5 ? n + 1 : 6);          /* plus one for 0‑based offset */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;              /* allocation failure */

        /* Initialize the first five known terms.  cache[0] is unused. */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 8;
        cache[3] = 14;
        cache[4] = 22;
        cache[5] = 28;
        max_computed = 5;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;      /* grow by factor 2 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;                /* reallocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 5) continue;                    /* base values already set */
        cache[i] = cache[i-2] + cache[i-3] - cache[i-5];
    }
    if (n > max_computed) max_computed = n;

    return cache[n];
}
