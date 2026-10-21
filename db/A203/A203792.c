/*
 * A203792  Number of (n+1)X5 0..3 arrays with every 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A203792: number of (n+1)×5 0..3 arrays with every 2×2 subblock having equal diagonal elements or equal anti‑diagonal elements.
   Empirical recurrence (valid for n ≥ 8):
        a(n) = 49*a(n-1) + 543*a(n-2) – 15435*a(n-3) + 26244*a(n-4) + 117288*a(n-5) – 79704*a(n-6) – 46656*a(n-7)
   Initial values:
        a(1)=38416 , a(2)=1911952 , a(3)=101071600 , a(4)=5426653360 ,
        a(5)=292394880208 , a(6)=15768587718160 , a(7)=850544485943152
*/

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Compute a(n) for n ≥ 1.  For n == 0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* size of allocated cache (indices 0 .. cap-1) */
    static idx_t len = 0;    /* greatest index that has been filled so far */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap   = 8;                 /* we need indices 1..7 as base cases */
        cache = seq_alloc(cap);    /* allocates an array of seq_t with size cap */
        cache[0] = 0;              /* unused – keeps indices aligned with OEIS offset */
        cache[1] = (seq_t)38416;
        cache[2] = (seq_t)1911952;
        cache[3] = (seq_t)101071600;
        cache[4] = (seq_t)5426653360ULL;
        cache[5] = (seq_t)292394880208ULL;
        cache[6] = (seq_t)15768587718160ULL;
        cache[7] = (seq_t)850544485943152ULL;
        len = 7;                   /* highest index filled */
    }

    /* grow cache if required */
    if ((size_t)n >= cap) {
        size_t newcap = n + 1;
        cache = seq_realloc(cache, cap, newcap); /* realloc relocates and resizes the array */
        cap = newcap;
    }

    /* compute missing terms up to n */
    if (len < n) {
        for (idx_t i = len + 1; i <= n; ++i) {
            if (i <= 7) continue;   /* base values already set */
            cache[i] = 49 * cache[i-1];
            cache[i] += 543 * cache[i-2];
            cache[i] -= 15435 * cache[i-3];
            cache[i] += 26244 * cache[i-4];
            cache[i] += 117288 * cache[i-5];
            cache[i] -= 79704 * cache[i-6];
            cache[i] -= 46656 * cache[i-7];
        }
        len = n;
    }

    return cache[n];
}
