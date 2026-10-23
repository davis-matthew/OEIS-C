/*
 * A178976  Number of collinear triples in graph of preceding terms.
 * https://oeis.org/A178976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts collinear triples of indices and values in the
   sequence itself.
   For each k < n all triples (i,j,k) with i<j<k, j-i=k-j and
   a(i),a(j),a(k) in arithmetic progression are counted. */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* cache[i] = a(i) for computed i */
    static idx_t cache_len = 0;   /* number of elements stored */
    static seq_t cnt = 0;         /* a(current_index-1) */

    /* Ensure cache can hold entries up to n */
    idx_t new_len = n + 1;
    size_t new_cap = (size_t)new_len;
    if (!cache) {                                 /* first allocation */
        cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
    } else if (new_len > cache_len) {             /* grow cache */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    }

    /* Compute a(0) if not yet done */
    idx_t start = cache_len;
    if (start == 0) {
        cache[0] = (seq_t)0;   /* a(0) = 0 */
        cnt = (seq_t)0;
        start = 1;
    }

    /* Incrementally compute a(idx) for idx = start .. n */
    for (idx_t idx = start; idx < new_len; ++idx) {
        idx_t m = idx - 1;        /* the largest index (k) in the triple */
        seq_t val_new = cnt;      /* start from previous a(idx-1) */

        /* Count new triples with k = m */
        for (idx_t i = 0; i + 2 <= m; ++i) {
            if (((m - i) & 1ULL) == 0) {            /* j must be integer */
                idx_t j = (i + m) >> 1;             /* j = (i+m)/2 */
                /* test arithmetic progression of values */
                if (cache[j] - cache[i] == cache[m] - cache[j]) {
                    val_new++;                      /* one more triple */
                }
            }
        }

        cnt = val_new;               /* a(idx) */
        cache[idx] = cnt;
    }

    cache_len = new_len;
    return cache[n];
}
