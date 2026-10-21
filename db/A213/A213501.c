/*
 * A213501  Number of (w,x,y) with all terms in {0,...,n} and w != max(|w-x|,
 *          |x-y|).
 * https://oeis.org/A213501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0)=0, a(1)=4, a(2)=16, a(3)=45, a(4)=94, a(5)=172, a(6)=281, a(7)=433
   a(n)=a(n-1)+2*a(n-2)-a(n-3)-2*a(n-4)-a(n-5)+2*a(n-6)+a(n-7)-a(n-8)  (n≥8)  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initial values */
    static const seq_t init[8] = {
        (seq_t)0, (seq_t)4, (seq_t)16, (seq_t)45,
        (seq_t)94, (seq_t)172, (seq_t)281, (seq_t)433
    };

    if (n < 8)
        return init[n];

    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;          /* allocation failed – should not happen */
        cache = new_cache;

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 8) {
                cache[i] = init[i];
            } else {
                /* compute using the recurrence, accumulating positives first
                   to avoid under‑flow for unsigned seq_t types */
                seq_t pos = cache[i-1];
                pos += (seq_t)2 * cache[i-2];
                pos += (seq_t)2 * cache[i-6];
                pos += cache[i-7];

                seq_t neg = cache[i-3];
                neg += (seq_t)2 * cache[i-4];
                neg += cache[i-5];
                neg += cache[i-8];

                cache[i] = pos - neg;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
