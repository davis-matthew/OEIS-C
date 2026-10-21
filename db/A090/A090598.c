/*
 * A090598  a(n) = 5^(n-1/2) * (2*n-1)!! * Integral_{x = 0..1/2} 1/(1+x^2)^(n
 *          + 1/2) dx.
 * https://oeis.org/A090598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n+2) = (-80*n^2 - 40*n)*a(n) + (18*n + 14)*a(n+1) a(1) = 1,
 *            a(2) = 14 (Offset: 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised terms, 1‑based */
static idx_t cap   = 0;       /* allocated capacity (in terms) */
static idx_t len   = 0;       /* highest index computed so far */

/* Ensure capacity for at least 'min_cap' terms */
static void grow_cache(idx_t min_cap) {
    idx_t newcap = cap ? cap * 2 : 16;
    while (newcap < min_cap) newcap *= 2;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cap   = newcap;
}

/* ------------------------------------------------------------- */
/* a(n) : main sequence accessor (external linkage)              */
/* ------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* below offset – unused */

    /* allocate storage if necessary */
    if (cap < n + 1) grow_cache(n + 1);

    /* initialise first two terms if not already in cache */
    if (len < 1) { cache[1] = 1; len = 1; }
    if (len < 2) { cache[2] = 14; len = 2; }

    /* grow the cache up to 'n' using the recurrence */
    for (idx_t i = len + 1; i <= n; ++i) {
        idx_t k = i - 2;                                  /* recurrence index */
        /* coefficients are non‑negative */
        uint64_t coeff1 = 18ULL * k + 14ULL;              /* (18*k+14)   */
        uint64_t coeff2 = 80ULL * k * k + 40ULL * k;      /* (80*k^2+40*k) */

        /* term calculations – use seq_t arithmetic */
        seq_t term1 = (seq_t)coeff1 * cache[i - 1];       /* (18*k+14)*a(k+1) */
        seq_t term2 = (seq_t)coeff2 * cache[i - 2];       /* (80*k^2+40*k)*a(k) */

        /* recurrence: a(k+2) = term1 - term2 (term1 ≥ term2) */
        cache[i] = term1 - term2;
    }
    len = n;           /* up to 'n' now computed */
    return cache[n];
}
