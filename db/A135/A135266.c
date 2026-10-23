/*
 * A135266  Partial sums of A132357.
 * https://oeis.org/A135266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(n+1)=3*a(n)+add[n mod 6] where add=[1,2,4,3,2,0]
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to support arbitrary call order and reuse previously computed values. */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;

    static const seq_t add[6] = { (seq_t)1, (seq_t)2, (seq_t)4,
                                  (seq_t)3, (seq_t)2, (seq_t)0 };

    /* Initialise cache with a(0)=0. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – return zero for safety */
        cache[0] = (seq_t)0;
        max_n = 0;
    }

    /* If n is larger than any previously computed index, extend and fill the cache. */
    if (n > max_n) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return 0; /* allocation failure – return zero for safety */
        cache = tmp;
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = prev * (seq_t)3 + add[(i - 1) % 6];
        }
        max_n = n;
    }

    return cache[n];
}
