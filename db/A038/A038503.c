/*
 * A038503  Sum of every 4th entry of row n in Pascal's triangle, starting at
 *          "n choose 0".
 * https://oeis.org/A038503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = Σ_{k=0..⌊n/4⌋} binomial(n,4k) Recurrence:
 *            a(0)=a(1)=a(2)=a(3)=1, a(n)=4*a(n-1)-6*a(n-2)+4*a(n-3) for n>=4
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* for the first four indices the value is 1 */
    if (n < 4) return 1;

    size_t target = (size_t)n;

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure */
        for (size_t i = 0; i < 4; ++i)
            cache[i] = 1;
    }

    /* enlarge cache if necessary */
    if (target >= cache_len) {
        size_t new_len = target + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        cache = tmp;
        for (size_t i = cache_len; i <= target; ++i) {
            /* recurrence: a(i)=4*a(i-1)-6*a(i-2)+4*a(i-3) */
            cache[i] = 4 * cache[i-1] - 6 * cache[i-2] + 4 * cache[i-3];
        }
        cache_len = new_len;
    }

    return cache[target];
}
