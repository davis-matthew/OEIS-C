/*
 * A053311  Partial sums of A000285.
 * https://oeis.org/A053311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=a(n-1)+a(n-2)+4; a(0)=1, a(1)=5
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size in elements */
    static idx_t len   = 0;   /* number of valid elements in cache */

    /* Ensure capacity for at least n+1 elements */
    if (n + 1 > cap) {
        idx_t newcap = (cap == 0) ? 2 : cap;
        while (newcap <= n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* Initialise cache if it is empty */
    if (len == 0) {
        cache[0] = (seq_t)1;  /* a(0) */
        len = 1;
    }

    /* Ensure second element */
    if (len == 1) {
        cache[1] = (seq_t)5;  /* a(1) */
        len = 2;
    }

    /* Compute terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + (seq_t)4;
    }
    len = n + 1;

    return cache[n];
}
