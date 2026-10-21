/*
 * A210674  a(0)=1; thereafter a(n) = 3*Sum_{k=1..n} binomial(2n,2k)*a(n-k).
 * https://oeis.org/A210674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; thereafter a(n) = 3*Sum_{k=1..n} binomial(2n,2k)*a(n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation across calls */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;      /* number of slots allocated (indices 0..cap-1) */

    /* Determine new capacity needed and resize if necessary */
    size_t old_cap = cap;
    size_t new_cap = (size_t)n + 1;        /* we need indices 0 .. n inclusive */

    if (new_cap > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms from the smallest index that was not yet
       computed (old_cap) up to n.  If old_cap is 0, the loop will start
       at index 0. */
    for (idx_t i = (idx_t)old_cap; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = 0;
        /* Sum over k = 1 .. i of binomial(2*i, 2*k) * a[i-k] */
        for (idx_t k = 1; k <= i; ++k) {
            idx_t two_i = i + i;
            idx_t two_k = k + k;
            seq_t bin  = binomial(two_i, two_k);     /* binomial(2i, 2k) */
            seq_t prod = bin * cache[i - k];
            sum += prod;
        }

        cache[i] = sum * (seq_t)3;
    }

    return cache[(idx_t)n];
}
