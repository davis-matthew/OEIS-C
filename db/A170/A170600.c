/*
 * A170600  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = 13*(a(n-1)+a(n-2)+…+a(n-47)) –
 *            91*a(n-48) + b(n) where b(0)=1, b(n)=2 for 1≤n≤47, b(48)=1,
 *            b(n)=0 otherwise
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n) {
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t len = 0;      /* number of elements stored in cache   */
    static idx_t cap = 0;      /* allocated capacity                    */

    /* helper to grow the cache if needed */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    while (len <= n) {
        /* compute a(len) from the recurrence */
        idx_t i = len;
        seq_t val = 0;

        if (i == 0) {
            val = 1;   /* b(0) = 1 */
        } else {
            /* sum 13*a(i-k) for k = 1..47 (or up to i) */
            for (idx_t k = 1; k <= 47 && k <= i; ++k) {
                val += (seq_t)13 * cache[i - k];
            }
            /* subtract 91*a(i-48) if i ≥ 48 */
            if (i >= 48) {
                val -= (seq_t)91 * cache[i - 48];
            }
            /* add b(i) */
            if (i <= 47) {
                val += 2;
            } else if (i == 48) {
                val += 1;
            }
        }

        /* ensure capacity */
        if (len + 1 > cap) {
            cap *= 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[len++] = val;
    }
    return cache[n];
}
