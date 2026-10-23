/*
 * A170209  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170209
    Number of reduced words of length n in the Coxeter group on 8 generators
    S_i with relations (S_i)^2 = (S_i S_j)^40 = I.

    The ordinary generating function is

        P(t) / Q(t)

    where

        P(t) = t^40 + 2 t^39 + 2 t^38 + … + 2 t + 1
        Q(t) = 21 t^40 – 6 t^39 – 6 t^38 – … – 6 t + 1

    From P(t)/Q(t) = Σ a(n) t^n we obtain the linear recurrence
    (valid for all n ≥ 0)

        a(n) = P_n + 6 * Σ_{k=1}^{min(39,n)} a(n‑k) – 21 * a(n‑40)

    with
        P_0 = 1,   P_1 … P_39 = 2,   P_40 = 1,   P_n = 0 for n > 40.

    This recurrence has order 40 and can be used to compute all terms
    efficiently.  The function below stores the sequence in a static
    cache so that arbitrary order of calls is supported.
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;   /* array a[0 … len-1] */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t len   = 0;      /* number of terms stored */

    /* if the requested index is already in the cache */
    if (n < (idx_t)len)
        return cache[n];

    /* enlarge the cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t newcap = cap ? cap << 1 : 128;
        while (newcap < needed)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory – terminate */
        if (cache)
            memcpy(tmp, cache, len * sizeof(seq_t));
        free(cache);
        cache = tmp;
        cap   = newcap;
    }

    /* compute terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        size_t maxk = i < 39 ? i : 39;        /* k = 1 … min(39,i) */
        for (size_t k = 1; k <= maxk; ++k)
            sum += cache[i - k];

        seq_t term = 6 * sum;                 /* 6*Σ a(i-k) */
        if (i >= 40)
            term = term - 21 * cache[i - 40]; /* –21*a(i-40) */

        /* add the P_i value */
        seq_t P;
        if (i == 0 || i == 40)
            P = 1;
        else if (i <= 39)
            P = 2;
        else
            P = 0;
        term += P;

        cache[i] = term;
    }
    len = needed;
    return cache[n];
}
