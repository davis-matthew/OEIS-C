/*
 * A072371  a(0) = 0, a(1) = 1, a(n+1) = 2*a(n) + (2*n-1)^2*a(n-1).
 * https://oeis.org/A072371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2; a(n) = 2*a(n-1) + ( 2*(n-1) - 1 )^2 * a(n-2) for
 *            n >= 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to allow arbitrary call order */
    static seq_t *cache = NULL;   /* cache[0..m] holds a(0)..a(m) */
    static idx_t cached_upto = 0; /* highest index stored */

    /* initialise cache for the base cases */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return 0;          /* out‑of‑memory fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cached_upto = 1;
    }

    /* expand cache if necessary */
    if (n > cached_upto) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;            /* fallback on allocation failure */
        cache = tmp;
        /* compute new terms */
        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            seq_t a_im1 = cache[i-1];
            seq_t a_im2 = cache[i-2];
            idx_t k = i - 1;                 /* k = n-1 in the recurrence */
            seq_t factor = (seq_t)(2 * k - 1); /* (2*(n-1)-1) */
            seq_t factor_sq = factor * factor; /* squared factor */
            cache[i] = (seq_t)2 * a_im1 + factor_sq * a_im2;
        }
        cached_upto = n;
    }

    return cache[n];
}
