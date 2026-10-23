/*
 * A178742  Partial sums of floor(2^n/9).
 * https://oeis.org/A178742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A178742: Partial sums of floor(2^n / 9).  
   Recurrence (for n > 5):  a(n) = a(n-6) + 7*2^(n-5) - 3.  
   Initial values:  a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=4.  */
#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;

    /* initial capacity: enough for first 6 terms */
    if (cap == 0)
    {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 1;
        cache[5] = 4;
        sz = 6;                     /* number of initialized entries */
    }

    /* grow cache if needed */
    while (n >= cap)
    {
        size_t newcap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) { free(cache); cache = NULL; return 0; } /* out of memory */
        cache = tmp;
        cap = newcap;
    }

    /* compute missing entries up to n */
    while (sz <= n)
    {
        idx_t i = sz;
        seq_t val;
        if (i <= 5)
        {
            /* already initialized, just keep them */
            cache[i] = cache[i]; /* no-op */
        }
        else
        {
            /* recurrence: a(i) = a(i-6) + 7 * 2^(i-5) - 3 */
            seq_t term = pow2((idx_t)(i - 5));   /* 2^(i-5) */
            term = term * (seq_t)7;              /* 7 * 2^(i-5) */
            term = term - (seq_t)3;              /* subtract 3 */
            term = term + cache[i - 6];          /* cache[i-6] + ... */
            val = term;
            cache[i] = val;
        }
        sz++;
    }

    return cache[n];
}
