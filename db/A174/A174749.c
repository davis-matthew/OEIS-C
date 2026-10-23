/*
 * A174749  x-values in the solution to x^2-34*y^2=1.
 * https://oeis.org/A174749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 70*a(n-1)-a(n-2), a(1)=1, a(2)=35 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    if (!cache) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;      /* a(1) */
        cache[1] = (seq_t)35;     /* a(2) */
        len   = 2;
    }

    if (n <= len) return cache[n - 1];

    /* Ensure we have room for n terms */
    if (n > cap) {
        size_t new_cap = cap ? cap : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap   = new_cap;
    }

    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 2]; /* a(i-1) */
        seq_t prev2 = cache[i - 3]; /* a(i-2) */
        cache[i - 1] = prev1 * (seq_t)70 - prev2;
    }
    len = n;
    return cache[n - 1];
}
