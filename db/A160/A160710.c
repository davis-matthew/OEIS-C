/*
 * A160710  E.g.f.: Sum_{n>=0} 2^(n^2)*log(1+x)^n/n!.
 * https://oeis.org/A160710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling1(n, k)*2^(k^2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"


seq_t a(idx_t n)
{
    /* Static state for memoisation and intermediate values */
    static idx_t max_n   = 0;      /* highest n already computed   */
    static size_t cap    = 0;      /* current allocation capacity */
    static seq_t *a_cache   = NULL;  /* a_cache[i] = a(i)         */
    static seq_t *prev      = NULL;  /* Stirling numbers of row n-1 */
    static seq_t *curr      = NULL;  /* Stirling numbers of row n   */
    static seq_t *pow2sq    = NULL;  /* pow2sq[k] = 2^(k^2)         */

    /* a(0) is always 1 */
    if (n == 0u)
        return (seq_t)1;

    /* Initial allocation on first call */
    if (a_cache == NULL) {
        cap = 1;
        a_cache = (seq_t *)malloc(cap * sizeof(seq_t));
        prev    = (seq_t *)malloc(cap * sizeof(seq_t));
        curr    = (seq_t *)malloc(cap * sizeof(seq_t));
        pow2sq  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!a_cache || !prev || !curr || !pow2sq)
            abort(); /* allocation failure */

        a_cache[0] = (seq_t)1;    /* a(0)   */
        prev[0]    = (seq_t)1;    /* Stirling(0,0) */
        pow2sq[0]  = (seq_t)1;    /* 2^(0^2)   */
        max_n = 0;
    }

    /* Grow the shared arrays if a larger n is requested */
    if ((size_t)n + 1u > cap) {
        size_t new_cap = (size_t)n + 1u;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        prev    = (seq_t *)realloc(prev,    new_cap * sizeof(seq_t));
        curr    = (seq_t *)realloc(curr,    new_cap * sizeof(seq_t));
        pow2sq  = (seq_t *)realloc(pow2sq,  new_cap * sizeof(seq_t));
        if (!a_cache || !prev || !curr || !pow2sq)
            abort();
        cap = new_cap;
    }

    /* Compute rows from (max_n + 1) up to n */
    for (idx_t m = max_n + 1u; m <= n; ++m) {
        /* 2^(m^2)  */
        pow2sq[m] = pow_uint(2ULL, (idx_t)((unsigned long long)m * (unsigned long long)m));

        /* Fill Stirling(n,k) for k = 0..m (unsigned values) */
        curr[0] = (seq_t)0;
        curr[m] = prev[m - 1u];                          /* s(n,n) = 1 */
        if (m >= 2u) {
            for (idx_t k = m - 1u; k > 1u; --k)          /* 2 <= k <= m-1 */
                curr[k] = prev[k] * (seq_t)(m - 1u) + prev[k - 1u];
            curr[1] = prev[1] * (seq_t)(m - 1u) + prev[0];
        } else {                                         /* m == 1 */
            curr[1] = prev[0];                            /* s(1,1) = 1 */
        }

        /* a(m) = Sum_{k=0..m} (-1)^(m-k) * s(n,k) * 2^(k^2)  */
        seq_t sum_pos = (seq_t)0;
        seq_t sum_neg = (seq_t)0;
        for (idx_t k = 0u; k <= m; ++k) {
            seq_t prod = curr[k] * pow2sq[k];
            if (((m - k) & 1u) == 0u)
                sum_pos += prod;       /* even sign */
            else
                sum_neg += prod;       /* odd sign  */
        }
        a_cache[m] = sum_pos - sum_neg;  /* non‑negative result */

        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    max_n = n;
    return a_cache[n];
}
