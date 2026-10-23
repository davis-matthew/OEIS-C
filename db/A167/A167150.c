/*
 * A167150  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the recurrence derived from the generating
 *            function. For n < 14: a(0)=1, a(1)=21, a(2)=420, a(3)=8400,
 *            a(4)=168000, a(5)=3360000, a(6)=67200000, a(7)=1344000000,
 *            a(8)=26880000000, a(9)=537600000000, a(10)=10752000000000,
 *            a(11)=215040000000000, a(12)=4300800000000000,
 *            a(13)=8601600000...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* The function signature required by the build system */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array holding computed terms */
    static size_t cache_cap = 0;         /* allocated capacity of cache   */
    static idx_t last_computed = 0;      /* highest index computed so far */

    /* -------------------------------------------------------------
       Initialise the cache on the first call.
       ------------------------------------------------------------- */
    if (!cache) {
        /* allocate a small initial capacity */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        /* base terms a[0] … a[13] */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        for (idx_t i = 2; i <= 13; ++i)
            cache[i] = cache[i-1] * (seq_t)20;
        last_computed = 13;
    }

    /* -------------------------------------------------------------
       Grow the cache if the requested index is beyond the current
       allocated capacity.  The growth factor is 2 for simplicity.
       ------------------------------------------------------------- */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* -------------------------------------------------------------
       Return immediately if the value has been computed before.
       ------------------------------------------------------------- */
    if (n <= last_computed)
        return cache[n];

    /* -------------------------------------------------------------
       Compute all missing terms up to n.
       ------------------------------------------------------------- */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 13; ++k)
            sum += ((seq_t)19) * cache[i - k];

        if (i == 14) {
            /* a(14) = 1 + Σ_{i=1}^{13}19*a(14-i) – 190*a(0) */
            cache[i] = sum - ((seq_t)190) * cache[0] + (seq_t)1;
        } else {
            /* a(n) = Σ_{i=1}^{13}19*a(n-i) – 190*a(n-14)  (n>14) */
            cache[i] = sum - ((seq_t)190) * cache[i - 14];
        }
    }

    last_computed = n;
    return cache[n];
}
