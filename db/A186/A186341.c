/*
 * A186341  a(n)=sum{k=0..floor(n/2), binomial(n-k,k)*A186338(k)}.
 * https://oeis.org/A186341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum{ k=0..⌊n/2⌋ } binomial(n-k,k)*A186338(k) Recurrence
 *            (OEIS A186341): (n+2)*a(n) + 5*(-n-1)*a(n-1) + 2*(-n+3)*a(n-2) +
 *            (38*n-59)*a(n-3) + (-22*n+41)*a(n-4) + 4*(-22*n+81)*a(n-5) +
 *            3*(19*n-79)*a(n-6) + 3*(29*n-164)*a(n-7) + 2*(-17*n+98)*a(n-8) +
 *            16*(-2*n+15)*a(n-9) = 0 Initial values: a(0...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     // for malloc, realloc

/* Cached values of a(n).  The cache grows only forward.  */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static idx_t  a_cache_last = 9;   /* last index known (initial data up to 9) */

static const seq_t a_initial[10] = {
    (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)15,
    (seq_t)33, (seq_t)95, (seq_t)237, (seq_t)667, (seq_t)1765
};

seq_t a(idx_t n)
{
    if (n < 10)
        return a_initial[n];

    /* Initialise cache if not yet done.  */
    if (!a_cache)
    {
        a_cache_cap = 10;
        a_cache = (seq_t*)malloc(sizeof(seq_t) * a_cache_cap);
        if (!a_cache) exit(EXIT_FAILURE);
        for (idx_t i = 0; i < 10; ++i)
            a_cache[i] = a_initial[i];
        a_cache_last = 9;
    }

    /* Grow cache if needed.  */
    if (a_cache_cap <= (size_t)n)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, sizeof(seq_t) * new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* Compute missing values up to n.  */
    for (idx_t i = a_cache_last + 1; i <= n; ++i)
    {
        idx_t m = i;                     /* shorthand */
        seq_t posSum = (seq_t)0;
        seq_t negSum = (seq_t)0;
        seq_t coeff;

        /* k = 1: coeff = -5*(m+1)  → negative → add to negSum */
        coeff = (seq_t)(5 * (m + 1));
        negSum += a_cache[i - 1] * coeff;

        /* k = 2: coeff = -2*(m-3)  → negative → add to negSum */
        coeff = (seq_t)(2 * (m - 3));
        negSum += a_cache[i - 2] * coeff;

        /* k = 3: coeff = 38*m-59  → positive → add to posSum */
        coeff = (seq_t)(38 * m - 59);
        posSum += a_cache[i - 3] * coeff;

        /* k = 4: coeff = -(22*m-41)  → negative → add to negSum */
        coeff = (seq_t)(22 * m - 41);
        negSum += a_cache[i - 4] * coeff;

        /* k = 5: coeff = -88*m+324  → negative → add to negSum */
        coeff = (seq_t)(88 * m - 324);
        negSum += a_cache[i - 5] * coeff;

        /* k = 6: coeff = 57*m-237   → positive → add to posSum */
        coeff = (seq_t)(57 * m - 237);
        posSum += a_cache[i - 6] * coeff;

        /* k = 7: coeff = 87*m-492   → positive → add to posSum */
        coeff = (seq_t)(87 * m - 492);
        posSum += a_cache[i - 7] * coeff;

        /* k = 8: coeff = -(34*m-196) → negative → add to negSum */
        coeff = (seq_t)(34 * m - 196);
        negSum += a_cache[i - 8] * coeff;

        /* k = 9: coeff = -(32*m-240) → negative → add to negSum */
        coeff = (seq_t)(32 * m - 240);
        negSum += a_cache[i - 9] * coeff;

        /* RHS = negSum - posSum (always non‑negative for m ≥ 10) */
        seq_t rhs = negSum - posSum;
        a_cache[i] = rhs / ((seq_t)(m + 2));
    }

    a_cache_last = n;
    return a_cache[n];
}
