/*
 * A027181  a(n) = Lucas(n+4) - 2*(n+3).
 * https://oeis.org/A027181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n+4) - 2*(n+3)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas numbers: L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *lucas_cache = NULL;
    static idx_t lucas_cache_max = 0;   /* greatest index cached */

    /* initial cache for L(0) and L(1) */
    if (!lucas_cache) {
        lucas_cache = (seq_t *)calloc(2, sizeof(seq_t));
        if (!lucas_cache) return 0;       /* allocation failure: fallback 0 */
        lucas_cache[0] = (seq_t)2;
        lucas_cache[1] = (seq_t)1;
        lucas_cache_max = 1;
    }

    idx_t k = n + 4;                     /* we need L(n+4) */

    if (k > lucas_cache_max) {
        /* enlarge cache */
        seq_t *tmp = (seq_t *)realloc(lucas_cache, (k + 1) * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure: fallback 0 */
        lucas_cache = tmp;

        /* compute new Lucas values */
        for (idx_t i = lucas_cache_max + 1; i <= k; ++i) {
            if (i == 0) {
                lucas_cache[i] = (seq_t)2;
            } else if (i == 1) {
                lucas_cache[i] = (seq_t)1;
            } else {
                lucas_cache[i] = lucas_cache[i - 1] + lucas_cache[i - 2];
            }
        }

        lucas_cache_max = k;
    }

    seq_t lucas_val = lucas_cache[k];
    seq_t sub = (seq_t)2 * ((seq_t)n + (seq_t)3);
    return lucas_val - sub;
}
