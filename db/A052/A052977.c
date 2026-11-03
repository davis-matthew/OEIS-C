/*
 * A052977  Expansion of (1-x)(1+x)/(1 - x - x^2 - x^3 + x^5).
 * https://oeis.org/A052977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-3) + a(n-2) + a(n-1) - a(n-5) with initial values
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=5
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t* cache = NULL;     /* stores a(0), a(1), ... */
    static idx_t last = 0;          /* largest index currently computed */

    /* Initialize the cache on the first call */
    if (cache == NULL) {
        /* Enough space for the first 5 terms */
        cache = mem_new(seq_t, 5);
        if (!cache) exit(1);             /* allocation failure aborts the program */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
        last = 4;                        /* indices 0 through 4 are computed */
    }

    if (n <= last)
        return cache[n];

    /* Extend the cache to hold up to n */
    idx_t new_size = n + 1;              /* array indices 0..n inclusive */
    if (new_size > last + 1) {
        seq_t* tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp) exit(1);
        cache = tmp;
    }

    /* Compute terms from the next missing index up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        if (k < 5) {
            /* First five terms are predefined; nothing to do */
        } else {
            cache[k] = cache[k - 3] + cache[k - 2] + cache[k - 1] - cache[k - 5];
        }
    }

    last = n;
    return cache[n];
}
