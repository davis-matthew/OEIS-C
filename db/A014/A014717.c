/*
 * A014717  a(n) = (F(n+1) + L(n))^2 where F(n) are the Fibonacci numbers
 *          (A000045) and L(n) are the Lucas numbers (A000032).
 * https://oeis.org/A014717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(n+1) + L(n))^2 Recurrence: a(n) = 2*a(n-1) + 2*a(n-2)
 *            - a(n-3) for n >= 3 Initial values: a(0) = 9 a(1) = 4 a(2) = 25
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms */
static size_t nptr = 0;       /* number of terms currently stored */
static size_t ncap = 0;       /* capacity of the cache array */

seq_t a(idx_t n)
{
    /* initialise cache with the first three terms */
    if (cache == NULL) {
        size_t init_cap = 8;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)9;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)25;
        nptr = 3;
        ncap = init_cap;
    }

    if (n < nptr)            /* term already computed */
        return cache[n];

    /* grow cache until it contains a(n) */
    while (nptr <= n) {
        if (nptr >= ncap) {   /* double capacity when needed */
            size_t newcap = (ncap == 0) ? 8 : ncap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
            ncap = newcap;
        }

        /* recurrence: a(k) = 2*a(k-1) + 2*a(k-2) - a(k-3) */
        seq_t val = cache[nptr-1];
        val = val * 2;
        seq_t t = cache[nptr-2];
        t = t * 2;
        val = val + t;
        val = val - cache[nptr-3];

        cache[nptr] = val;
        ++nptr;
    }

    return cache[n];
}
