/*
 * A115167  Odd numbers k such that k-1 and k+1 have the same number of prime
 *          divisors with multiplicity.
 * https://oeis.org/A115167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth odd number k such that bigomega(k-1) ==
 *            bigomega(k+1)
 * Range:     u8 n <= 22, u16 n <= 4576, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset protection */

    /* static cache of previously found terms */
    static seq_t *cache = NULL;   /* array of found terms */
    static size_t cap = 0;        /* allocated capacity */
    static size_t len = 0;        /* number of terms stored */
    static idx_t cur = 5;         /* next odd candidate to test */

    /* ensure we have at least n terms in the cache */
    while (len < n) {
        seq_t om1 = bigomega(cur - 1);
        seq_t om2 = bigomega(cur + 1);

        if (om1 == om2) {
            /* need to grow the cache? */
            if (len == cap) {
                size_t newcap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(*cache));
                if (!tmp)
                    abort();  /* out of memory */
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = (seq_t)cur;
        }

        cur += 2; /* next odd number */
        /* overflow guard – practically unreachable for n ≤ 10000 */
        if (cur == 0)
            cur = 1;
    }

    return cache[n - 1];
}
