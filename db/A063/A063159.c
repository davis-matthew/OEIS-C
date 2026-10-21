/*
 * A063159  Dimension of the space of weight 2n cusp forms for Gamma_0( 91 ).
 * https://oeis.org/A063159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + a(n-3) - a(n-4) ,  n > 5
 * Initial values: a(1)=7 , a(2)=26 , a(3)=46 , a(4)=62 , a(5)=82
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache 1‑based (index 0 unused) */
    static seq_t *cache = NULL;
    static idx_t max_idx = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        idx_t init_size = 6;               /* indices 0..5 */
        cache = (seq_t*)malloc(sizeof(seq_t) * init_size);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)7;   /* a(1) */
        cache[2] = (seq_t)26;  /* a(2) */
        cache[3] = (seq_t)46;  /* a(3) */
        cache[4] = (seq_t)62;  /* a(4) */
        cache[5] = (seq_t)82;  /* a(5) */
        max_idx = 5;
    }

    if (n <= max_idx)
        return cache[n];

    /* ensure capacity */
    idx_t need = n + 1;               /* need indices up to n */
    if (need > max_idx) {
        /* increase to at least need; double to reduce reallocs */
        idx_t new_size = max_idx;
        while (new_size < need) new_size <<= 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
    }

    /* compute terms up to n */
    for (idx_t i = max_idx + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
    }
    max_idx = n;

    return cache[n];
}
