/*
 * A159336  Transform of the finite sequence (1, 0, -1) by the T_{1,0}
 *          transformation (see link).
 * https://oeis.org/A159336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) for n ≥ 5 a(0)=1, a(1)=2,
 *            a(2)=4, a(3)=11, a(4)=26
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t last = (idx_t)-1;

    if (cache == NULL) {
        /* initialize cache with first 5 terms */
        cap   = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* ensure allocation succeeded */
        if (!cache) abort();

        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        cache[3] = 11;
        cache[4] = 26;
        last = 4;
    }

    /* direct return for known initial values */
    if (n <= 4) {
        return cache[n];
    }

    /* extend capacity if necessary */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i-1] - 2 * cache[i-2] + cache[i-3];
    }
    last = n;
    return cache[n];
}
