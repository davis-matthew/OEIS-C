/*
 * A173098  a(1)=1, a(2)=2, a(n)=2*a(n-2)*a(n-1)-a(n-1).
 * https://oeis.org/A173098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A173098: a(1)=1, a(2)=2,
   a(n)=2*a(n-2)*a(n-1)-a(n-1)  for n>=3   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n<1, harmless */

    static seq_t *cache = NULL;          /* 1‑based array: cache[1], cache[2] ... */
    static size_t cap   = 0;             /* allocated size in elements   */
    static idx_t  latest = 0;            /* highest index computed so far */

    /* Resize cache if necessary so that it can hold index n */
    if (cap <= n) {
        size_t new_cap = cap ? cap : 4;   /* start with a small table */
        while (new_cap <= n)
            new_cap <<= 1;                /* double until it fits          */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Ensure the two initial terms are available   */
    if (latest < 2) {
        cache[1] = 1;
        cache[2] = 2;
        latest   = 2;
    }

    /* Return if already computed */
    if (n <= latest)
        return cache[n];

    /* Compute terms from latest+1 up to n */
    for (idx_t i = latest + 1; i <= n; ++i) {
        seq_t prev1  = cache[i - 1];
        seq_t prev2  = cache[i - 2];
        seq_t factor = 2 * prev2 - 1;       /* (2*a(i-2) - 1)          */
        cache[i] = prev1 * factor;          /* a(i) = a(i-1) * factor */
    }
    latest = n;
    return cache[n];
}
