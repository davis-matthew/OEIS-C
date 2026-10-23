/*
 * A046337  Odd numbers with an even number of prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A046337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   odd numbers whose total number of prime factors (counted with
 *            multiplicity) is even. Sequence A046337: a(1)=1, a(2)=9,
 *            a(3)=15, a(4)=21, a(5)=25,
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of computed terms  */
    static seq_t *cache      = NULL;
    static idx_t cacheSize   = 0;
    static idx_t cacheCount  = 0;
    static uint64_t lastChecked = 1; /* last odd number examined */

    /* Ensure we have at least n terms in the cache */
    while (cacheCount < n)
    {
        /* First term is 1 */
        if (cacheCount == 0)
        {
            if (cacheSize == 0)
            {
                cacheSize = 16;
                cache = (seq_t *)malloc(sizeof(seq_t) * cacheSize);
            }
            cache[cacheCount++] = (seq_t)1;
            lastChecked = 1;
            continue;
        }

        /* Next odd candidate */
        uint64_t cand = lastChecked + 2;
        int parity = 0;            /* 0 even, 1 odd */
        uint64_t tmp = cand;

        /* Simple trial division to count prime factors mod 2 */
        for (uint64_t p = 3; p * p <= tmp; p += 2)
        {
            while (tmp % p == 0)
            {
                parity ^= 1;
                tmp /= p;
            }
        }
        if (tmp > 1)                /* remaining prime factor */
            parity ^= 1;

        if (parity == 0)           /* even number of prime factors */
        {
            if (cacheCount == cacheSize)
            {
                cacheSize *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cacheSize);
            }
            cache[cacheCount++] = (seq_t)cand;
        }
        lastChecked = cand;
    }
    return cache[n - 1];
}
