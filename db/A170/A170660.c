/*
 * A170660  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * for n ≥ 1 use the linear recurrence derived from *
 *            * G(t) = (1 + 2t + 2t^2 + … + 2t^48 + t^49) / * (325t^49 -
 *            25t^48 - … - 25t - 1) * * i.e. * * a(n) = 25 * Σ_{k=1}^{48}
 *            a(n-k) – 325 * a(n-49) + coeff(n) * * where coeff(n) = 1 if n =
 *            0 or 49, * 2 if 1 ≤ n ≤ 48, * 0 otherwise. * * Thi...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values  */
static idx_t cache_size = 0;     /* capacity of `cache`        */
static idx_t last_computed = 0;  /* next index that has not yet been computed */

/* Ensure that values up to `n` have been computed and stored in `cache`. */
static void ensure_cache(idx_t n)
{
    if (n < last_computed)
        return;            /* nothing to do */

    /* Grow the cache if necessary. */
    if ((idx_t)(n + 1) > cache_size) {
        size_t new_cap = (size_t)(n + 1) * 2;      /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* fatal failure */
        cache = tmp;
        cache_size = new_cap;
    }

    /* Coefficients for the numerator of G(t). */
    const seq_t COEFF_0_49 = (seq_t)1;
    const seq_t COEFF_1_48 = (seq_t)2;

    /* For each new index, compute the recurrence value. */
    for (idx_t i = last_computed; i <= n; ++i) {
        seq_t sum = (seq_t)0;

        /* Σ_{k=1}^{48} a(i-k), stopping when k > i. */
        for (int k = 1; k <= 48 && (idx_t)k <= i; ++k) {
            sum += (seq_t)(25) * cache[i - k];
        }

        /* – 325·a(i-49) when i ≥ 49. */
        if (i >= 49) {
            sum -= (seq_t)(325) * cache[i - 49];
        }

        /* Add the constant term from the numerator. */
        if (i == 0 || i == 49) {
            sum += COEFF_0_49;
        } else if (i <= 48) {
            sum += COEFF_1_48;
        }
        /* for i ≥ 50, the coefficient is 0 (nothing to add). */

        cache[i] = sum;
    }

    last_computed = n + 1;
}

/* Public API. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_size = 16;                     /* arbitrary small start */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();                 /* fatal failure */
        cache[0] = (seq_t)1;                 /* a(0) = 1 */
        last_computed = 1;                   /* indices < 1 are ready */
    }

    ensure_cache(n);
    return cache[n];
}
