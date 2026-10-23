/*
 * A117439  Expansion of (1-x^2)/(1 -4*x -2*x^2 +x^4).
 * https://oeis.org/A117439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - a(n-4) Initial values: a(0)=1,
 *            a(1)=4, a(2)=17, a(3)=76
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;

    /* enlarge cache if necessary */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        } else if (cap < new_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (tmp) cache = tmp;
        }
        /* compute new entries */
        idx_t start = (cap == 0) ? 0 : cap;
        for (idx_t i = start; i <= n; ++i) {
            if (i == 0)          cache[i] = 1;
            else if (i == 1)     cache[i] = 4;
            else if (i == 2)     cache[i] = 17;
            else if (i == 3)     cache[i] = 76;
            else
                cache[i] = ((seq_t)4) * cache[i-1]
                         + ((seq_t)2) * cache[i-2]
                         -                cache[i-4];
        }
        cap = new_cap;
    }
    return cache[n];
}
