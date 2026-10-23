/*
 * A033978  Trajectory of 1 under map n->45n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)= (a(n-1) is odd ? 45*a(n-1)+1 : a(n-1)/2) */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to allow arbitrary call order and reuse of earlier terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initialise cache with a(0). */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* In production this should handle allocation failure. */
        cache[0] = 1;                  /* a(0) = 1 */
        cache_len = 1;
    }

    /* Grow cache to hold up to index n. */
    if ((size_t)(n + 1) > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* Allocation failure handling. */
        cache = tmp;
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t prev = cache[i - 1];
            if ((prev & (seq_t)1) == 1) {   /* odd */
                cache[i] = prev * (seq_t)45 + (seq_t)1;
            } else {                       /* even */
                cache[i] = prev / (seq_t)2;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
