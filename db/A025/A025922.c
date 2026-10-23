/*
 * A025922  Expansion of 1/((1-x^8)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A025922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   recurrence a(n) =
 *            a(n-8)+a(n-11)+a(n-12)-a(n-19)-a(n-20)-a(n-23)+a(n-31) with
 *            a(0)=1 and a(k)=0 for k<0 This counts partitions of n into parts
 *            8,11,12
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   // allocated size
    static idx_t max_n    = 0;    // highest n computed so far

    /* Return pre‑computed value if available */
    if (cache != NULL && n <= max_n)
        return cache[n];

    /* Allocation / reallocation */
    if (cache == NULL) {
        /* First invocation: allocate up to n */
        cache_cap = n + 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure – return zero */
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        cache[0] = 1;          /* a(0) */
        max_n = 0;
    } else if (n + 1 > cache_cap) {
        /* Grow the buffer; keep existing data */
        idx_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;     /* allocation failure – return zero */
        cache = tmp;
        cache_cap = new_cap;
        for (idx_t i = max_n + 1; i < cache_cap; ++i) cache[i] = 0;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 8)  val += cache[i-8];
        if (i >= 11) val += cache[i-11];
        if (i >= 12) val += cache[i-12];
        if (i >= 19) val -= cache[i-19];
        if (i >= 20) val -= cache[i-20];
        if (i >= 23) val -= cache[i-23];
        if (i >= 31) val += cache[i-31];
        cache[i] = val;
    }
    max_n = n;
    return cache[n];
}
