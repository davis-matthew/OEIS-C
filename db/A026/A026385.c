/*
 * A026385  Sum{T(n-k,k)}, 0<=k<=[ n/2 ], where T is the array in A026374.
 * https://oeis.org/A026385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 3*a(n-3) + a(n-4) for n>3 Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 80, u128 n <= 159,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* array of computed terms   */
    static idx_t last  = 0;        /* highest index currently cached   */
    static size_t cap   = 0;       /* allocated capacity     */

    /* first call: allocate and initialise first four terms  */
    if (cache == NULL) {
        cap = 16;
        cache = seq_alloc(cap);          /* seq_alloc handles proper construction */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        last = 3;
    }

    /* if the requested index is already computed, return it */
    if (n <= last)
        return cache[n];

    /* grow the cache if necessary */
    if ((idx_t)(cap) <= n) {
        size_t newcap = cap;
        while ((size_t)(newcap) <= (size_t)(n)) newcap <<= 1;
        cache = seq_realloc(cache, cap, newcap);
        cap = newcap;
    }

    /* compute terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* i-2, i-3, i-4 are all valid because i >= 4 here   */
        seq_t term = cache[i-2] + cache[i-4];
        term += cache[i-3] * ((seq_t)3);
        cache[i] = term;
    }

    last = n;
    return cache[n];
}
