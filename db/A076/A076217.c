/*
 * A076217  a(1)=1, a(n) = a(n-1) + n * sign(n-a(n-1)).
 * https://oeis.org/A076217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, look
 * Formula:   a(1)=1, a(n)=a(n-1)+n*sign(n-a(n-1)) (sign(0)=0, sign(>0)=1,
 *            sign(<0)=-1)
 * Range:     u8 n <= 243, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses memoisation: the sequence is computed incrementally up to the
   largest requested index and stored in a static array for future calls. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;         /* cache[i] holds a(i+1) */
    static idx_t cache_size = 0;        /* number of elements currently cached */

    if (n == 0) return 0;              /* offset protection – not used */

    /* Ensure the cache contains entries up to index n */
    if (cache_size < n) {
        idx_t new_size = n * 2;         /* grow factor for amortised O(1) per term */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) abort();           /* out‑of‑memory, best effort */
        /* initialise new entries */
        for (idx_t i = cache_size; i < n; ++i) {
            seq_t prev = (i == 0) ? 1 : cache[i-1];
            if (i == 0) {               /* a(1) is fixed */
                cache[0] = 1;
                continue;
            }
            seq_t idx = (seq_t)(i + 1); /* current n */
            seq_t sign = 0;
            if (idx > prev) sign = 1;
            else if (idx < prev) sign = -1;
            cache[i] = prev + idx * sign;
        }
        cache_size = n;
    }
    return cache[n-1];
}
