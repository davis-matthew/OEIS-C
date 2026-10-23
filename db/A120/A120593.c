/*
 * A120593  G.f. satisfies: 5*A(x) = 4 + x + A(x)^4, starting with [1,1,6].
 * https://oeis.org/A120593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   5*A(x) = 4 + x + A(x)^4 Recurrence (derived from the functional
 *            equation): a(n+3) = ( 8*(4n+5)*(2n+1)*(4n-1)*a(n) +
 *            64*(n+1)*(48n^2+96n+43)*a(n+1) + 6144*(2n+3)*(n+2)*(n+1)*a(n+2)
 *            ) / (491*(n+3)*(n+2)*(n+1)) Initial values: a(0)=1, a(1)=1,
 *            a(2)=6 The function below uses memoisation and works for...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper to grow the cache to at least `new_cap` elements */
static void grow_cache(idx_t new_cap, seq_t **arr, idx_t *cap)
{
    if (*cap >= new_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    /* realloc never fails when new_cap <= old cap; in case of failure
       we simply terminate (the problem environment guarantees enough
       memory for the required size). */
    *arr = tmp;
    *cap = new_cap;
}

/* Compute a(n+3) from a(n), a(n+1), a(n+2) using the recurrence. */
static inline seq_t compute_term(idx_t n, seq_t *data)
{
    /* ``n`` is the base index, compute a[n+3] */
    if (sizeof(seq_t) <= 8) {               /* use __int128 for safety */
        unsigned __int128 coeff1 = (unsigned __int128)8 *
                                   (4 * n + 5) *
                                   (2 * n + 1) *
                                   ((n == 0) ? 1 : (4 * n - 1));
        unsigned __int128 coeff2 = (unsigned __int128)64 *
                                   (n + 1) *
                                   (48 * n * n + 96 * n + 43);
        unsigned __int128 coeff3 = (unsigned __int128)6144 *
                                   (2 * n + 3) *
                                   (n + 2) *
                                   (n + 1);
        unsigned __int128 denom = (unsigned __int128)491 *
                                  (n + 3) *
                                  (n + 2) *
                                  (n + 1);

        unsigned __int128 term1 =
            coeff1 * (unsigned __int128)(data[n]);
        unsigned __int128 term2 =
            coeff2 * (unsigned __int128)(data[n + 1]);
        unsigned __int128 term3 =
            coeff3 * (unsigned __int128)(data[n + 2]);

        unsigned __int128 num;
        if (n == 0)
            num = term2 + term3 - term1;   /* subtract for n==0 */
        else
            num = term1 + term2 + term3;

        return (seq_t)(num / denom);
    } else {                               /* use the arithmetic of the
                                            * arbitrary‑precision type */
        seq_t term1;
        if (n == 0) {
            /* coefficient is negative for n==0 */
            term1 = -( (seq_t)8 * (4 * n + 5) * (2 * n + 1) *
                       (seq_t)1 * data[n] );
        } else {
            term1 = (seq_t)8 * (4 * n + 5) * (2 * n + 1) *
                    (seq_t)(4 * n - 1) * data[n];
        }
        seq_t term2 = (seq_t)64 * (n + 1) *
                      (48 * n * n + 96 * n + 43) *
                      data[n + 1];
        seq_t term3 = (seq_t)6144 * (2 * n + 3) *
                      (n + 2) * (n + 1) *
                      data[n + 2];
        seq_t denominator = (seq_t)491 * (n + 3) *
                            (n + 2) * (n + 1);
        seq_t num = term1 + term2 + term3;
        return num / denominator;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    /* Initialise the cache with the first three terms on first call. */
    if (cache_size == 0) {
        cache_cap   = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache_size = 3;
    }

    /* Return cached value if already available. */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow cache and compute terms up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i >= cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 3;
            while (new_cap <= i) new_cap <<= 1;
            grow_cache(new_cap, &cache, &cache_cap);
        }

        /* Compute a(i) using the recurrence (i = n+3 with n = i-3). */
        idx_t base = i - 3;
        cache[i] = compute_term(base, cache);
        ++cache_size;
    }

    return cache[n];
}
