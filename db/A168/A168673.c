/*
 * A168673  Binomial transform of A169609.
 * https://oeis.org/A168673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) for n > 2 Initial values:
 *            a(0) = 1, a(1) = 4, a(2) = 10
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* in-place memoisation */
    static seq_t *cache = NULL;
    static idx_t len   = 0;   /* number of entries currently stored */
    static size_t cap  = 0;   /* allocated capacity in cache */

    /* Handle the first three terms directly without allocating */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;
    if (n == 2)
        return (seq_t)10;

    /* initialise cache on first use */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)10;
        len = 3;
    }

    /* grow cache until we have term a(n) */
    while (len <= n) {
        if (len >= cap) {                 /* double the allocation */
            cap *= 2;
            cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        }

        /* compute a(len) using the recurrence */
        seq_t a1 = cache[len-1];
        seq_t a2 = cache[len-2];
        seq_t a3 = cache[len-3];

        /* a_n = 3*(a_{n-1} - a_{n-2}) + 2*a_{n-3}   */
        seq_t diff = a1 - a2;            /* non‑negative because the sequence is increasing */
        seq_t val  = 3 * diff;           /* 3*(a_{n-1}-a_{n-2}) */
        val += 2 * a3;                   /* + 2*a_{n-3} */

        cache[len] = val;
        len++;
    }

    return cache[n];
}
