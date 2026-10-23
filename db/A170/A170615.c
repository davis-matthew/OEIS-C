/*
 * A170615  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A170615: Number of reduced words of length n in the Coxeter group on
   30 generators with relations (S_i)^2 = (S_i S_j)^48 = I.
   Recurrence derived from the generating function:
       a(0) = 1
       for 1 ≤ n ≤ 47  : a(n) - 28 * Σ_{k=1}^{n  } a(n-k) = 2
       for n = 48      : a(48) - 28 * Σ_{k=1}^{47} a(48-k) + 406 * a(0) = 1
       for n ≥ 49      : a(n) - 28 * Σ_{k=1}^{47} a(n-k) + 406 * a(n-48) = 0
   (Σ denotes the sum of the indicated terms.)  This recurrence
   generates the sequence given in the OEIS entry. */

#include <stdlib.h>

/* static cache for memoised values */
static seq_t *cache = NULL;          /* array of computed a(n) */
static idx_t cache_size = 0;         /* number of valid entries in cache */

/* ensure the cache can hold indices up to 'upto' */
static void
expand_cache(idx_t upto)
{
    if (upto + 1 <= cache_size) return;      /* already large enough */

    idx_t new_size = upto + 1;                /* we need indices 0..upto inclusive */

#ifdef __cplusplus                    /* C++: use new/delete, copy explicitly */
    seq_t *new_data = new seq_t[new_size];
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_data[i] = cache[i];
        delete [] cache;
    }
    cache = new_data;
#else                                 /* C: use realloc, which preserves data */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = tmp;
#endif
    /* newly allocated elements are uninitialised; they will be computed later */
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* compute from current cache size up to n */
    idx_t start = cache_size;              /* first index that is not yet known */
    expand_cache(n);                       /* ensure we have space for 0..n */

    for (idx_t i = start; i <= n; ++i) {
        seq_t r;                           /* right‑hand side contribution from the numerator */
        if (i == 0 || i == 48)           /* coefficient 1 at t^0 and t^48 */
            r = 1;
        else if (i <= 47)                /* coefficient 2 for 1 ≤ i ≤ 47 */
            r = 2;
        else
            r = 0;                       /* coefficient 0 for i > 48 */

        seq_t sum = 0;                   /* Σ_{k=1}^{min(47,i)} a(i-k) */
        for (idx_t k = 1; k <= 47 && i >= k; ++k)
            sum += cache[i - k];

        seq_t val = r + ((seq_t)28 * sum);
        if (i >= 48)
            val -= ((seq_t)406 * cache[i - 48]);

        cache[i] = val;
    }
    return cache[n];
}
