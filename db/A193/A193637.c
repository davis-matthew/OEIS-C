/*
 * A193637  a(n) = a(n-1)^2 - n^(n+1).
 * https://oeis.org/A193637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1)^2 - n^(n+1); a(0) = 0
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 6, i64 n <= 7, i128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static cache to store already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements stored */

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        size_t new_len = (size_t)(n + 1);          /* ensure space for 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { /* memory error: return 0 */
            return (seq_t)0;
        }
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)0;
            } else {
                seq_t prev = cache[i - 1];
                seq_t sq   = prev * prev;          /* a(i-1)^2 */
                seq_t base = (seq_t)i;
                seq_t exp  = (seq_t)(i + 1);
                seq_t pw   = big_pow(base, exp);   /* i^(i+1) */
                cache[i] = sq - pw;                /* recurrence */
            }
        }
        cache_size = (idx_t)new_len;
    }

    return cache[n];
}
