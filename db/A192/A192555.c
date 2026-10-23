/*
 * A192555  a(n) = Sum_{k=0..n} Stirling2(n+1, k+1)*(-1)^(n-k)*k!^2.
 * https://oeis.org/A192555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n+1, k+1)*(-1)^(n-k)*(k!)^2 (OEIS
 *            A192555)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
/* use new/delete for C++ to construct seq_t objects */
#  define ALLOC(type,n)   new type[(n)]
#  define FREE(p)         delete[] (p)
#else
/* use malloc/free for C, implicit cast accepted */
#  define ALLOC(type,n)   ((type*)malloc((n)*sizeof(type)))
#  define FREE(p)         free((p))
#endif

seq_t a(idx_t n)
{
    /* Static cache for Stirling rows and computed a-values */
    static seq_t *S = NULL;        /* current Stirling row S(i+1, j) (j=0..i+1) */
    static size_t S_size = 0;      /* length of S array */
    static seq_t *a_cache = NULL; /* cached a(n) values */
    static size_t cache_cap = 0;   /* length of a_cache array */
    static idx_t max_n_computed = (idx_t)-1; /* highest n for which a_cache is valid */

    /* Grow cache if needed.  Copy old values when growing. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = ALLOC(seq_t, new_cap);
        if (a_cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_cache[i] = a_cache[i];
            FREE(a_cache);
        }
        a_cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialisation on first call */
    if (S == NULL) {
        /* Start with row for n = 0: S(1,1) = 1 */
        S_size = 2;
        S = ALLOC(seq_t, S_size);
        S[0] = (seq_t)0;   /* unused */
        S[1] = (seq_t)1;
        /* a(0) = 1 */
        a_cache[0] = (seq_t)1;
        max_n_computed = 0;
    }

    /* Compute values up to requested n if necessary */
    if (n > max_n_computed) {
        for (idx_t m = max_n_computed + 1; m <= n; ++m) {
            /* Build next Stirling row: S(m+1, j) = j*S(m, j) + S(m, j-1)   */
            size_t oldSize = S_size;           /* size of previous row  (== m+1) */
            size_t newSize = (size_t)m + 2;    /* size of next row      (== m+2) */
            seq_t *newS = ALLOC(seq_t, newSize);
            newS[0] = (seq_t)0;                /* not used */
            for (size_t j = 1; j < newSize; ++j) {
                seq_t term1 = (j < oldSize) ? S[j] * (seq_t)j : (seq_t)0;
                seq_t term2 = (j-1 < oldSize) ? S[j-1] : (seq_t)0;
                newS[j] = term1 + term2;
            }
            FREE(S);
            S = newS;
            S_size = newSize;

            /* Compute a(m) using the new row */
            seq_t sum = (seq_t)0;
            seq_t fact = (seq_t)1;            /* k! for current k */
            for (idx_t k = 0; k <= m; ++k) {
                seq_t term = S[(size_t)k + 1] * (fact * fact);
                if (((m - k) & 1) == 0)
                    sum += term;
                else
                    sum -= term;
                if (k != m)
                    fact *= (seq_t)(k + 1);  /* next factorial */
            }
            a_cache[m] = sum;
        }
        max_n_computed = n;
    }

    return a_cache[n];
}
