/*
 * A059356  A diagonal of triangle in A008298.
 * https://oeis.org/A059356
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n‑1)! · Σ_{k=1}^{n‑1} σ(k)·σ(n‑k)/k
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2  (so a(2) is the first term) */

#include <stdlib.h>          /* calloc, free */
#include "../utils/factorial.h"   /* seq_t factorial(idx_t n); */

 /* --------------------- */
/* Cache for σ(1)…σ(limit) */
static seq_t *sigma_cache = NULL;
static idx_t  sigma_limit = 0;

/* Recompute σ values up to 'limit' (inclusive).  Similar to a divisor‑sum sieve. */
static seq_t *ensure_sigma(idx_t limit)
{
    if (limit <= sigma_limit)
        return sigma_cache;

    /* Release old cache (if any) and allocate a new one. */
    free(sigma_cache);
    /* Allocate (limit+1) elements: 0..limit */
    sigma_cache = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    sigma_limit = limit;

    /* Classic a[k] += i for every multiple of i. */
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t si = (seq_t)i;
        for (idx_t j = i; j <= limit; j += i) {
            sigma_cache[j] += si;
        }
    }
    return sigma_cache;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* undefined below the offset */

    /* σ(1)…σ(n‑1) */
    seq_t *sigma = ensure_sigma(n - 1);

    /* (n‑1)!  */
    seq_t fact = factorial(n - 1);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n - 1; ++k) {
        seq_t term = fact;            /* (n‑1)!  */
        term /= (seq_t)k;             /* /(k)    */
        term *= sigma[k];             /* *σ(k)   */
        term *= sigma[n - k];         /* *σ(n‑k) */
        sum += term;                  /* accumulate */
    }
    return sum;
}
