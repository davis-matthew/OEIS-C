/*
 * A135370  a(1)=1; then if n even a(n) = n + a(n-1), if n odd a(n) = 2*n +
 *          a(n-1).
 * https://oeis.org/A135370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n>1, a(n)= (n even)? a(n-1)+n : a(n-1)+2*n
 * Range:     u8 n <= 18, u16 n <= 294, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache keeps already computed terms. */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms stored so far */

    if (n == 0) return 0; /* offset starts at 1, n=0 is unused */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – caller handles it */
        cache[0] = 1;                  /* a(1) */
        cached = 1;
    }

    /* Extend cache if needed. */
    if (n > cached) {
        idx_t new_size = cached * 2;
        if (new_size < n) new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – caller handles it */
        cache = tmp;

        for (idx_t i = cached + 1; i <= n; ++i) {
            seq_t prev = cache[i - 2];                 /* a(i-1) */
            seq_t inc  = (i & 1) ? (seq_t)2 * (seq_t)i : (seq_t)i;
            cache[i - 1] = prev + inc;                  /* a(i) */
        }
        cached = n;
    }

    return cache[n - 1];
}
