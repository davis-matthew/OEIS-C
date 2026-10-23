/*
 * A041181  Denominators of continued fraction convergents to sqrt(101).
 * https://oeis.org/A041181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 20*a(n-1) + a(n-2); a(0) = 1, a(1) = 20
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS A041181.
   The implementation uses memoisation with a dynamically grown cache. */
seq_t a(idx_t n)
{
    /* Static memoisation storage */
    static seq_t *cache   = NULL;   /* array of computed terms   */
    static size_t  cap    = 0;      /* allocated capacity        */
    static idx_t   cached = 0;      /* number of terms already computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 4;                           /* initial capacity          */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* out‑of‑memory guard       */
        cache[0] = (seq_t)1;                /* a(0)                     */
        cache[1] = (seq_t)20;               /* a(1)                     */
        cached = 2;                         /* we have 0 and 1 cached   */
    }

    /* If the requested index is already cached, return it directly */
    if (n < cached)
        return cache[n];

    /* Ensure the array can hold terms up to n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out‑of‑memory guard */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute and store missing terms up to n */
    for (idx_t i = cached; i <= n; ++i) {
        /* recurrence uses previously stored terms */
        cache[i] = ((seq_t)20 * cache[i - 1]) + cache[i - 2];
    }
    cached = n + 1;          /* update number of cached terms */

    return cache[n];
}
