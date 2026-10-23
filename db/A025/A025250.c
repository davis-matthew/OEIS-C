/*
 * A025250  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-3)*a(3) for n >= 4,
 *          with initial terms 0, 1, 1, 0.
 * https://oeis.org/A025250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-3)*a(3), with
 *            initial values a(1)=0, a(2)=1, a(3)=1, a(4)=0
 * Range:     u8 n <= 16, u16 n <= 26, u32 n <= 44, u64 n <= 78, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms: cache[0] = a(1), cache[1] = a(2), ... */
    static seq_t *cache = NULL;
    static idx_t size = 0;      /* number of terms currently stored          */
    static idx_t capacity = 0;  /* allocated capacity                       */

    /* Ensure cache can hold at least n terms (indices 0 .. n-1) */
    if (size < n) {
        /* Grow capacity geometrically */
        size_t new_cap = capacity ? capacity : 8;
        while (new_cap < (size_t)n) new_cap <<= 1;
        if (new_cap > capacity) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            capacity = new_cap;
        }
        /* Compute all new terms */
        for (idx_t i = size; i < n; ++i) {
            if (i < 4) {                     /* terms 1 .. 4 */
                if (i == 0) cache[i] = (seq_t)0;   /* a(1) */
                else if (i == 1) cache[i] = (seq_t)1;/* a(2) */
                else if (i == 2) cache[i] = (seq_t)1;/* a(3) */
                else cache[i] = (seq_t)0;     /* a(4) */
            } else {
                /* Recurrence: a(i+1) = Σ_{k=1}^{(i-1)} a(k) * a(i+1-k)  */
                /* In zero‑based indices: i = n-1, second index = i - k */
                seq_t sum = 0;
                for (idx_t k = 1; k <= i - 2; ++k) {
                    sum += cache[k - 1] * cache[i - k];
                }
                cache[i] = sum;
            }
        }
        size = n;
    }
    return cache[n - 1];
}
