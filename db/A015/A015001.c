/*
 * A015001  q-factorial numbers for q=3.
 * https://oeis.org/A015001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} (3^k - 1)/2, a(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   pow3, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n < cache_len) {
        return cache[n];
    }

    idx_t new_len = n + 1;
    seq_t *tmp = seq_realloc(cache, cache_len, new_len);
    if (!tmp) {
        exit(1);  /* Allocation failure */
    }
    cache = tmp;

    for (idx_t k = cache_len; k < new_len; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;  /* a(0) = 1 */
        } else {
            seq_t pow3k = pow3(k);               /* 3^k              */
            seq_t term  = (pow3k - (seq_t)1) / (seq_t)2;  /* (3^k-1)/2       */
            cache[k] = cache[k-1] * term;          /* a(k) = a(k-1)*term */
        }
    }

    cache_len = new_len;
    return cache[n];
}
