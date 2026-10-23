/*
 * A017884  Expansion of 1/(1-x^9-x^10-x^11-x^12-x^13-x^14-x^15-x^16-x^17).
 * https://oeis.org/A017884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9)+a(n-10)+a(n-11)+a(n-12)+a(n-13)+
 *            a(n-14)+a(n-15)+a(n-16)+a(n-17) (for n>16) Initial values:
 *            a(0)=1, a(1)=…=a(8)=0, a(9)=…=a(17)=1
 * Range:     u8 n <= 45, u16 n <= 76, u32 n <= 139, u64 n <= 264,
 *            u128 n <= 515, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation array */
    static seq_t *cache      = NULL;   /* store values up to the max n seen */
    static size_t cache_len = 0;       /* current number of stored elements */
    size_t req_len           = (size_t)(n + 1); /* number of elements needed */

    /* Grow the cache if necessary */
    if (req_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, req_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* memory allocation failed */
        cache      = tmp;
        /* initialise any new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i < 9) {
                cache[i] = (seq_t)0;
            } else if (i < 18) {
                cache[i] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                for (idx_t k = 9; k <= 17; ++k) {
                    sum += cache[i - k];
                }
                cache[i] = sum;
            }
        }
        cache_len = req_len;
    }

    return cache[n];
}
