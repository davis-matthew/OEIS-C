/*
 * A056900  Numbers n where 36n^2+36n+11 is prime.
 * https://oeis.org/A056900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Enumerates integers n such that 36*n^2 + 36*n + 11 is prime.
 * a(n) = the n‑th such integer (offset 0): a(0)=0, a(1)=1, ...
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static storage for memoising the found n values */
    static seq_t *cache = NULL;   /* array of results      */
    static size_t cap   = 0;      /* allocated capacity    */
    static size_t sz    = 0;      /* number of stored terms*/
    static idx_t next_m = 0;      /* next candidate m     */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory */
    }

    /* generate terms until we have at least n+1 entries */
    while (sz <= n) {
        seq_t m = (seq_t)next_m;
        /* f(m) = 36*m^2 + 36*m + 11 */
        seq_t f = 36 * (m * m) + 36 * m + (seq_t)11;
        if (isPrimeGeneric(f)) {
            if (sz == cap) {            /* grow cache if needed */
                cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
                if (tmp == NULL) return 0; /* out of memory */
                cache = tmp;
            }
            cache[sz++] = m;
        }
        ++next_m; /* move to next candidate m */
    }

    return cache[n];
}
