/*
 * A160013  Numerator of Hermite(n, 9/25).
 * https://oeis.org/A160013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 9/25) Recurrence: a(0)=1, a(1)=18, a(n)
 *            = 18*a(n-1) - 1250*(n-1)*a(n-2) (for n ≥ 2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------------------- Utility includes ---------------------- */
#include <stdlib.h>

/* ---------------------- Sequence function ---------------------- */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;

    /* Static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of computed terms    */
    static size_t cap        = 0;      /* current capacity           */
    static idx_t  last_n     = (idx_t)-1; /* largest n computed yet   */

    /* Initialise cache if this is the first call */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();      /* out‑of‑memory protection */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)18;
        last_n  = 1;
    }

    /* Return cached value if already computed */
    if (last_n >= n) return cache[n];

    /* Grow cache capacity if necessary */
    while (cap <= (size_t)n) {
        cap = (cap == 0) ? 2 : cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        /* next = 18*a(i-1) - 1250*(i-1)*a(i-2) */
        seq_t term1 = (seq_t)18 * cache[i - 1];
        seq_t term2 = (seq_t)1250 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = term1 - term2;
    }

    last_n = n;               /* update largest computed index   */
    return cache[n];
}
