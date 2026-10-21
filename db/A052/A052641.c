/*
 * A052641  Expansion of e.g.f. (1-x)/(1-3*x-x^2+x^3).
 * https://oeis.org/A052641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+3) = 3*(n+3)*a(n+2) + (6+5*n+n^2)*a(n+1) * -
 *            (6+11*n+6*n^2+n^3)*a(n) * Initial values: a(0)=1, a(1)=2,
 *            a(2)=14
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* dynamically‑grown array of terms   */
static idx_t cache_len = 0;     /* number of terms currently stored   */
static size_t cache_cap = 0;    /* allocated capacity                  */

/* Ensure that the cache holds terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;

    /* Grow capacity if necessary */
    if ((size_t)(n+1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)(n+1)) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc may return NULL only if size is zero – never seen here */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise new entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) { cache[0] = (seq_t)1; continue; }
        if (i == 1) { cache[1] = (seq_t)2; continue; }
        if (i == 2) { cache[2] = (seq_t)14; continue; }

        /* recurrence written with 1‑based indices inside the loop */
        idx_t ii = i;                  /* current index */
        idx_t n3 = ii - 3;             /* n in the recurrence      */

        /* convert to seq_t for safe multiplication */
        seq_t sn3 = (seq_t)n3;

        /* term1 = 3*(n+3)*a(n+2) = 3*ii*a[ii-1] */
        seq_t coeff1 = (seq_t)3 * (seq_t)ii;
        seq_t term1 = coeff1 * cache[ii-1];

        /* term2 = (6 + 5*n + n^2) * a[n+1] */
        seq_t coeff2 = (seq_t)6 + (seq_t)5 * sn3 + sn3 * sn3;
        seq_t term2 = coeff2 * cache[ii-2];

        /* term3 = (6 + 11*n + 6*n^2 + n^3) * a[n] */
        seq_t coeff3 = (seq_t)6 + (seq_t)11 * sn3 +
                       (seq_t)6 * sn3 * sn3 + sn3 * sn3 * sn3;
        seq_t term3 = coeff3 * cache[ii-3];

        cache[ii] = term1 + term2 - term3;
    }
    cache_len = n + 1;   /* number of stored terms */
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
