/*
 * A127218  Half-indexed Lucas numbers second version L(n)=A000032=Lucas
 *          numbers a(0)=2, a(1)=2, a(2)=1, a(3)=2, a(4)=3, a(5)=3,
 *          a(2n)=L(n), for n>2: a(2n+1)=L(n)+L(n-3)=2*L(n-1) for n>5:
 *          a(n)+a(n+2)=a(n+4) a(2n)=L(n), so a(n)=L(n/2).
 * https://oeis.org/A127218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 46, u32 n <= 92, u64 n <= 184,
 *            u128 n <= 368, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=2, a(1)=2, a(2)=1, a(3)=2, a(4)=3, a(5)=3, a(6)=4, a(7)=6,
   a(8)=7, a(9)=8, a(10)=11
   a(n) = a(n-2) + a(n-4)   for n > 10
   (OEIS A127218) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static idx_t  last   = (idx_t)-1;   /* index of the largest pre‑computed value */

    /* Ensure space for the first 11 values */
    if (cap < 11) {
        size_t newcap = 11;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failed */
        cache = tmp;
        cap   = newcap;
        /* initialise the 11 known terms */
        cache[0] = 2; cache[1] = 2; cache[2] = 1; cache[3] = 2;
        cache[4] = 3; cache[5] = 3; cache[6] = 4; cache[7] = 6;
        cache[8] = 7; cache[9] = 8; cache[10] = 11;
        last = 10;
    }

    if (n < 11)                /* already known */
        return cache[(size_t)n];

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n + 1)
            newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i >= 11)
            cache[(size_t)i] = cache[(size_t)(i - 2)] + cache[(size_t)(i - 4)];
    }
    last = n;
    return cache[(size_t)n];
}
