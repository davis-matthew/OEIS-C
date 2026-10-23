/*
 * A011664  A binary m-sequence: expansion of reciprocal of x^5+x^3+1.
 * https://oeis.org/A011664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1 a(n) = a(n-3) + a(n-5)
 *            (mod 2) for all n ≥ 5 The sequence is the binary expansion of
 *            1/(x^5+x^3+1) over GF(2)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;  /* store computed terms (0 or 1)          */
static size_t cache_len = 0;    /* number of terms stored (≥ 5 after init)*/
static size_t cache_cap = 0;    /* allocated capacity                    */

seq_t a(idx_t n)
{
    /* Ensure the cache has space for indices up to n */
    size_t needed = (size_t)n + 1;
    size_t new_cap = (needed > 5 ? needed : 5);   /* at least 5 for init */
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first five terms if not already done */
    if (cache_len < 5) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)1;
        cache_len = 5;
    }

    /* Compute any missing terms up to n using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* i ≥ 5, so i-3 and i-5 are valid indices */
        cache[i] = cache[i-3] ^ cache[i-5];
    }
    cache_len = (size_t)n + 1;   /* update the number of computed terms */

    return cache[(size_t)n];
}
