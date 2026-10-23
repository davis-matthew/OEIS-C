/*
 * A172434  G.f.: Sum_{n>=0} a(n)*x^n/n!^4 = [ Sum_{n>=0} x^n/n!^4 ]^3.
 * https://oeis.org/A172434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)^4 * Sum_{j=0..k} C(k,j)^4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void ensure_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap > 0 ? *cap * 2 : 1);
    while (new_cap < need)
        new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Static memoisation tables */
    static seq_t *S = NULL;      /* S[k] = Sum_{j=0..k} C(k,j)^4 */
    static size_t S_cap = 0;
    static idx_t S_max = 0;      /* largest k whose S has been computed */

    static seq_t *A = NULL;      /* A[k] = a(k) */
    static size_t A_cap = 0;
    static idx_t A_max = 0;      /* largest k whose A has been computed */

    /* Ensure capacity for S and A up to index n */
    ensure_cap(&S, &S_cap, n + 1);
    ensure_cap(&A, &A_cap, n + 1);

    /* Initialise base terms (safe to rewrite each call) */
    S[0] = 1;
    A[0] = 1;

    /* Compute S(k) for k = S_max+1 .. n */
    for (idx_t k = S_max + 1; k <= n; ++k)
    {
        seq_t comb = 1;
        seq_t sum  = 0;
        for (idx_t j = 0; j <= k; ++j)
        {
            if (j > 0) {
                /* comb = C(k, j) */
                comb = comb * (k - j + 1) / j;
            }
            seq_t p = comb;
            p = p * p;   /* square once */
            p = p * p;   /* square again -> power 4 */
            sum += p;
        }
        S[k] = sum;
    }
    if (n > S_max) S_max = n;

    /* Compute a(m) for m = A_max+1 .. n using the computed S */
    for (idx_t m = A_max + 1; m <= n; ++m)
    {
        seq_t comb = 1;
        seq_t sum  = 0;
        for (idx_t k = 0; k <= m; ++k)
        {
            if (k > 0) {
                /* comb = C(m, k) */
                comb = comb * (m - k + 1) / k;
            }
            seq_t p = comb;
            p = p * p;   /* square once */
            p = p * p;   /* square again -> power 4 */
            sum += p * S[k];
        }
        A[m] = sum;
    }
    if (n > A_max) A_max = n;

    return A[n];
}
