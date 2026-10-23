/*
 * A117261  Row sums of triangle A117260.
 * https://oeis.org/A117261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) * 2^(n-1) + 1,  a(0) = 1
 * (Row sums of triangle A117260)
 */
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;          /* highest n for which cache is filled   */
    /* Allocate or extend cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure: return 0 */
        cache[0] = (seq_t)1;
        if (n == 0) return cache[0];
        cached_n = 0;
    } else if (n > cached_n) {
        /* Reallocate larger array */
        size_t new_size = (n + 1) * sizeof(seq_t);
        seq_t *tmp = (seq_t *)malloc(new_size);
        if (tmp == NULL) return 0;   /* allocation failure: return 0 */
        memcpy(tmp, cache, (cached_n + 1) * sizeof(seq_t));
        free(cache);
        cache = tmp;
        /* Compute missing values */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t pow = pow2(i - 1);
            cache[i] = cache[i - 1] * pow + (seq_t)1;
        }
        cached_n = n;
    }
    return cache[n];
}
