/*
 * A163290  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from the generating function of A163290:
   a(0)=1, a(1)=50, a(2)=2450, a(3)=120050, a(4)=5881225,
   a(n)=48*a(n-1)+48*a(n-2)+48*a(n-3)-1176*a(n-4) for n≥5   */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values */
    static size_t cap    = 0;     /* current capacity of cache */
    static idx_t next    = 0;     /* first index not yet computed (equal to cap) */

    /* grow cache if necessary */
    if ((size_t)n + 1 > cap)
    {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing values */
    for (idx_t i = next; i <= n; ++i)
    {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = cache[0] * (seq_t)48 + (seq_t)2;
        else if (i == 2)
            cache[2] = cache[1] * (seq_t)48 + cache[0] * (seq_t)48 + (seq_t)2;
        else if (i == 3)
            cache[3] = cache[2] * (seq_t)48 + cache[1] * (seq_t)48 + cache[0] * (seq_t)48 + (seq_t)2;
        else if (i == 4)
            cache[4] = cache[3] * (seq_t)48 + cache[2] * (seq_t)48
                     + cache[1] * (seq_t)48 - cache[0] * (seq_t)1176 + (seq_t)1;
        else
            cache[i] = cache[i-1] * (seq_t)48 + cache[i-2] * (seq_t)48
                     + cache[i-3] * (seq_t)48 - cache[i-4] * (seq_t)1176;
    }

    next = (idx_t)cap;   /* all indices up to current cap are computed */
    return cache[n];
}
