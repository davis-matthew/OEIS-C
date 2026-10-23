/*
 * A164706  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A164706
    Number of reduced words of length n in Coxeter group on 5 generators
    S_i with relations S_i^2 = (S_i S_j)^8 = I.
    G.f.: (t^8 + 2*t^7 + 2*t^6 + 2*t^5 + 2*t^4 + 2*t^3 + 2*t^2 + 2*t + 1) /
           (6*t^8 - 3*t^7 - 3*t^6 - 3*t^5 - 3*t^4 - 3*t^3 - 3*t^2 - 3*t + 1)

    Recurrence (derived from the denominator of the G.f.):

        a(n) = 3*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7))
               - 6*a(n-8) + p(n)

    where
        p(0) = 1,
        p(n) = 2 for 1 ≤ n ≤ 7,
        p(8) = 1,
        p(n) = 0 for n ≥ 9.
    For negative indices a(k)=0, and the recurrence is valid for all n≥0.
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t last  = (size_t)-1;   /* highest index that has a value */

    /* Allocate or grow the cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap ? cap * 2 : (size_t)(n + 1);
        if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
#if defined(__cplusplus)
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) return (seq_t)0;     /* allocation failed – return 0 as a safe fallback */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n (inclusive) */
    size_t start = last + 1;            /* next index to compute */
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; k <= 7; ++k) {
            if (i >= k) sum += cache[i - k];
        }

        seq_t term = ((seq_t)3) * sum;               /* 3 * (a(i-1)+...+a(i-7)) */
        if (i >= 8) term -= ((seq_t)6) * cache[i - 8]; /* - 6 * a(i-8) */

        /* Add the contribution from the numerator polynomial */
        if (i == 0 || i == 8)          term += ((seq_t)1);
        else if (i >= 1 && i <= 7)     term += ((seq_t)2);
        /* else term += 0 */

        cache[i] = term;
    }
    last = (size_t)n;
    return cache[n];
}
