/*
 * A177172  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=10, k=0 and l=-2.
 * https://oeis.org/A177172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177172:
   a(0) = 1, a(1) = 10
   a(n+1) = sum_{p=0}^{n} a(p)*a(n-p) - 2   for n >= 1   (k = 0, l = -2) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored */

    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    /* initialise cache if empty */
    if (cache_len == 0) {
        cache = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache_len = 2;
    }

    /* grow cache to hold up to index n */
    if (n >= cache_len) {
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure – abort */
            abort();
        }
        cache = tmp;

        /* compute missing terms */
        for (idx_t m = cache_len; m <= n; ++m) {
            seq_t sum = 0;
            idx_t last = m - 1;
            for (idx_t p = 0; p <= last; ++p) {
                sum += cache[p] * cache[last - p];
            }
            sum -= (seq_t)2;          /* subtract l = -2 */
            cache[m] = sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}
