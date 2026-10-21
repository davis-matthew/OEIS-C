/*
 * A056133  a(1) = 1, a(m+1) = sum_{k=1 to m}[min(m, a(k))].
 * https://oeis.org/A056133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for m≥1, a(m+1)=∑_{k=1}^{m} min(m, a(k)) */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t size = 0;      /* number of terms currently computed */
    static idx_t capacity = 0;  /* allocated array capacity */

    /* Resize the cache if necessary */
    if (n > capacity) {
        idx_t new_cap = capacity;
        if (new_cap == 0) new_cap = 1;
        while (new_cap < n) new_cap <<= 1;  /* grow by factor 2 */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Compute terms up to n */
    if (n > size) {
        /* Initialize first term if this is the first call */
        if (size == 0) {
            cache[0] = (seq_t)1;
            size = 1;
        }

        for (idx_t i = size; i < n; ++i) {
            /* i corresponds to m (since a(m+1) is at index i) */
            seq_t sm = (seq_t)i;   /* m as seq_t */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t x = cache[k];
                sum += (x < sm ? x : sm);
            }
            cache[i] = sum;
        }
        size = n;
    }

    return cache[n - 1];
}
