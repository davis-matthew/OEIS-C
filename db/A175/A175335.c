/*
 * A175335  a(1)=1. For n>1, a(n) equals the smallest number > a(n-1) of the
 *          form a(k) U j U a(k) U j, where U represents concatenation
 *          (written in decimal) of the binary representation of the
 *          arguments, where 1 <= k < n, and j = {0} or {1}.
 * https://oeis.org/A175335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1. For n > 1, a(n) = smallest x > a(n-1) of the form a(k)
 *            U j U a(k) U j, where 1 ≤ k < n and j ∈ {0,1}
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"          /* provided by the harness */

/*-------------------------------------------*/
/*  Cached implementation of a(n)            */
/*-------------------------------------------*/
seq_t a(idx_t n)
{
    /* Static cache: 1‑based index, cache[0] unused */
    static seq_t *cache      = NULL;   /* values of a(i) */
    static size_t cap       = 0;       /* allocated size (bytes)   */
    static idx_t  last      = 0;       /* highest index computed  */

    if (n < 1)  /* undefined domain outside the offset */
        return (seq_t)0;

    /* Initialise cache lazily                     */
    if (cache == NULL) {
        cap  = 64;                       /* initial capacity (in seq_t's) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        cache[0] = 0;                    /* unused slot 0  */
        cache[1] = (seq_t)1;            /* a(1) = 1      */
        last    = 1;
    }

    /* Grow cache if needed                       */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap << 1 : 1;
        while ((size_t)n + 1 > newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* If the needed value is already cached, return it */
    if (n <= last)
        return cache[n];

    /* -------------------------------------------------- */
    /*  Compute missing terms from last + 1 up to n       */
    /* -------------------------------------------------- */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t best = 0;
        bool found = false;

        /* Iterate over all smaller indices k   */
        for (idx_t k = 1; k < i; ++k) {
            seq_t ak  = cache[k];
            unsigned len = bitlen_seq_t(ak);        /* bit length of a(k) */

            /* Two possible values of j (0 or 1) */
            for (idx_t jj = 0; jj <= 1; ++jj) {
                seq_t part = (ak << 1) | (seq_t)jj;    /* a(k) concatenated with j */

                /* Construct candidate: part U part */
                /* candidate = part * 2^(len+1) + part  */
                seq_t shifted = part << (len + 1);
                seq_t cand = shifted | part;

                if (cand > prev) {
                    if (!found || cand < best) {
                        best  = cand;
                        found = true;
                    }
                }
            }
        }
        /* For completeness, for the first step (i=2) the loop above
           covers all possible k < 2, i.e. k=1.  If no candidate is
           found (which can only happen when the arithmetic overflows
           seq_t), we leave the element as 0.  That situation can
           only arise for a seq_t that is too small to hold the next
           term, and such a build is expected to test only terms that
           fit into seq_t. */

        cache[i] = best;
    }

    last = n;
    return cache[n];
}
