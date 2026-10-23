/*
 * A020943  a(2n+1) = |a(2n) - a(2n-1)|, a(2n) = a(n) + a(n-1).
 * https://oeis.org/A020943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(2n) = a(n) + a(n-1) Formula: a(2n+1) = |a(2n) - a(2n-1)|
 *            (offset 1, a(1)=0)
 * Range:     u8 n <= 5461, u16 n <= 5461, u32 n <= 5461, u64 n <= 5461,
 *            u128 n <= 5461, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t last  = 0;

    /* grow cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);
        if (cache) {
            for (idx_t i = 0; i <= last; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute all required values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)0;
        } else if (i == 2) {
            cache[2] = (seq_t)1;
        } else if ((i & 1) == 0) {            /* even */
            idx_t k = i >> 1;
            cache[i] = cache[k] + cache[k - 1];
        } else {                              /* odd */
            seq_t a_even = cache[i - 1];      /* a(2k)   */
            seq_t a_prev = cache[i - 2];      /* a(2k-1) */
            cache[i] = (a_even > a_prev) ? (a_even - a_prev) : (a_prev - a_even);
        }
    }
    last = n;
    return cache[n];
}
