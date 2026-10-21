/*
 * A057688  Trajectory of 5 under the '5x+1' map.
 * https://oeis.org/A057688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A057688 a(0) = 5 a(n) = a(n-1)/2 if a(n-1) is even a(n-1)/3 if
 *            a(n-1) is odd and divisible by 3 5*a(n-1) + 1 otherwise
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* cache the sequence values up to the largest requested n */
    static seq_t *cache = NULL;        /* dynamically growable array of terms   */
    static idx_t cached_len = 0;       /* number of terms currently stored (1 => a(0) only) */

    /* initialise cache with a(0) = 5 if it hasn't been done yet */
    if (cached_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* fallback for out‑of‑memory */
        cache[0] = (seq_t)5;
        cached_len = 1;
    }

    /* grow cache to cover index n */
    if (n + 1 > cached_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;        /* fallback */
        cache = tmp;
        /* compute new terms up to n */
        for (idx_t i = cached_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            if ((prev & (seq_t)1) == 0) {               /* even */
                cache[i] = prev / (seq_t)2;
            } else if (prev % (seq_t)3 == 0) {          /* odd, divisible by 3 */
                cache[i] = prev / (seq_t)3;
            } else {                                   /* odd, not divisible by 3 */
                cache[i] = prev * (seq_t)5 + (seq_t)1;
            }
        }
        cached_len = new_len;
    }

    return cache[n];
}
