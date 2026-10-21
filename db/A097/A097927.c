/*
 * A097927  If a(n-1) is divisible by 2 then a(n) = a(n-1)/2. If a(n-1) is not
 *          divisible by 2 then a(n) = 47-(a(n-1)+1)/2; a(0)=1.
 * https://oeis.org/A097927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; if a(n-1) even then a(n)=a(n-1)/2 else a(n)=47-(a(n-1)+1)/2 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;   /* array of already computed terms */
    static idx_t  cache_len = 0;  /* number of elements stored (len of array) */

    /* allocate first element if necessary */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure: return 0 */
        cache[0] = (seq_t)1;                  /* a(0) = 1 */
        cache_len = 1;
    }

    /* extend cache up to n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure: return 0 */
        cache = tmp;

        for (idx_t i = cache_len; i < new_len; ++i) {
            seq_t prev = cache[i - 1];
            if ((prev & (seq_t)1) == 0)
                cache[i] = prev >> 1;                /* even */
            else
                cache[i] = (seq_t)47 - ((prev + (seq_t)1) >> 1); /* odd */
        }
        cache_len = new_len;
    }

    return cache[n];
}
