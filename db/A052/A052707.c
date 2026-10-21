/*
 * A052707  Odd powers of 2 multiplied by Catalan numbers.
 * https://oeis.org/A052707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(2*n-1)*Cat(n-1) for n>0, a(0)=0 Recurrence: a(1)=2,
 *            a(n+1) = 8*(2n-1)*a(n)/(n+1) (n≥1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised with a static cache so that any call order is
   handled efficiently.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cachelen = 0;

    if (cache == NULL) {
        /* initialise cache for indices 0 and 1 */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL)
            return 0;                     /* out of memory: undefined behaviour */
        cache[0] = 0;
        cache[1] = 2;
        cachelen = 2;
    }

    /* extend cache up to n */
    if (n >= cachelen) {
        size_t newlen = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                     /* out of memory: undefined behaviour */
        cache = tmp;

        for (idx_t k = cachelen; k <= n; ++k) {
            /* recurrence a(k) = 8*(2k-3)*a(k-1) / k   for k>=2
               (since a(1)=2 already stored) */
            seq_t prev = cache[k - 1];
            uint64_t factor_small = 8ULL * (2 * k - 3);    /* 8*(2k-3) fits 64 bit for k≤10000 */

            if (sizeof(seq_t) <= sizeof(unsigned long long)) {
                /* use 128‑bit intermediate to avoid overflow in the
                   64‑bit build.  */
                unsigned __int128 temp = (unsigned __int128)prev * factor_small;
                cache[k] = (seq_t)(temp / k);
            } else {
                /* arbitrary‑precision build – rely on operator* and / */
                cache[k] = (prev * (seq_t)factor_small) / (seq_t)k;
            }
        }
        cachelen = newlen;
    }

    return cache[n];
}
