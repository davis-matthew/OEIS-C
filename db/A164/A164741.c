/*
 * A164741  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164741:
   a(0)=1, a(1)=6, a(2)=30, a(3)=150, a(4)=750, a(5)=3750,
   a(6)=18750, a(7)=93750, a(8)=468735.
   For n ≥ 9  : a(n)=4*(a(n-1)+…+a(n-7)) – 10*a(n-8). */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* stored terms */
    static idx_t  cap   = 0;           /* allocated capacity (powers of 2) */
    static idx_t  last  = 0;           /* number of valid entries */

    /* Initial values for indices 0 … 7. */
    static const seq_t init[8] = {1, 6, 30, 150, 750, 3750, 18750, 93750};

    /* Maximum term needed for a request < 8 */
    if (n < 8) {
        if (!cache) {
            idx_t newcap = 8;
            if (newcap < 64) newcap = 64;
            cache = seq_realloc(NULL, 0, newcap);
            if (!cache) abort();
            cap = newcap;
            for (idx_t i = 0; i < 8; ++i) cache[i] = init[i];
            last = 8;
        }
        return cache[n];
    }

    /* Ensure the cache exists and is large enough for indices up to n. */
    if (!cache) {
        idx_t need = n + 1;
        if (need < 8) need = 8;
        idx_t newcap = 64;
        while (newcap < need) newcap <<= 1;
        cache = seq_realloc(NULL, 0, newcap);
        if (!cache) abort();
        cap = newcap;
        for (idx_t i = 0; i < 8; ++i) cache[i] = init[i];
        last = 8;
    } else if (cap <= n) {
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = seq_realloc(cache, cap, newcap);
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 7; ++k) sum += cache[i - k];

        seq_t res = sum * 4;
        if (i == 8) {               /* special formula for a(8) */
            res -= cache[0] * 10;
            res += 1;
        } else {                    /* i ≥ 9 */
            res -= cache[i - 8] * 10;
        }
        cache[i] = res;
    }
    last = n + 1;
    return cache[n];
}
