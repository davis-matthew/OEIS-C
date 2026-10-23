/*
 * A145322  Numbers n such that there exists x in N : (x+1)^3-x^3=31*n^2.
 * https://oeis.org/A145322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=7, a(2)=170107, a(n)=24302*a(n-1)-a(n-2) for n>2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms, 1‑indexed in use */
    static idx_t  len   = 0;       /* number of terms currently cached   */
    static idx_t  cap   = 0;       /* current capacity of cache           */

    /* init cache with the first two known terms */
    if (len == 0) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)7        ;   /* a(1) */
        cache[1] = (seq_t)170107   ;   /* a(2) */
        len   = 2;
    }

    if (n == 0) return (seq_t)0;   /* undefined for offset 1, okay to return 0 */

    /* if already cached, return it */
    if (n <= len) return cache[n - 1];

    /* grow capacity if needed */
    if (n > cap) {
        /* double until enough */
        while (cap < n) cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = len; i < n; ++i) {
        /* cache[0] corresponds to a(1), so cache[i-2] is a(i-1) */
        seq_t next = (seq_t)24302 * cache[i - 1] - cache[i - 2];
        cache[i] = next;
    }
    len = n;

    return cache[n - 1];
}
