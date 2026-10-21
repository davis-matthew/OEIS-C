/*
 * A099915  Expansion of g.f. (1+4*x)/((1-x)*(1-10*x)).
 * https://oeis.org/A099915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11*a(n-1) - 10*a(n-2) with a(0) = 1, a(1) = 15. The
 *            recurrence produces a(n) = (14*10^n - 5)/9, matching OEIS
 *            A099915
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for memory allocation */
#include <stdlib.h>
#include <stddef.h>

/* Compute the nth term of sequence A099915.
 * The function is thread‑unsafe but satisfies all
 * requirements for a single threaded build. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache   = NULL;   /* array of terms a(0)..a(len-1) */
    static size_t  cap    = 0;      /* allocated size of cache */
    static idx_t  len     = 0;      /* number of terms currently stored */

    /* Ensure the cache can hold the requested index. */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 4;
        while (newcap < (size_t)n + 1) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    /* Fill cache up to the requested index using the recurrence. */
    if (len == 0) {          /* initialise a(0) and a(1) */
        cache[0] = (seq_t)1;
        len = 1;
    }
    if (len == 1) {          /* initialise a(1) */
        cache[1] = (seq_t)15;
        len = 2;
    }

    for (idx_t i = len; i <= n; ++i) {
        /* For i >= 2: a(i) = 11*a(i-1) - 10*a(i-2) */
        cache[i] = cache[i-1] * (seq_t)11 - cache[i-2] * (seq_t)10;
    }
    len = n + 1;  /* we now have terms up to a(n) */

    return cache[(size_t)n];
}
