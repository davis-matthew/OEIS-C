/*
 * A130979  G.f.: 7/(3 + 4*sqrt(1-28*x)).
 * https://oeis.org/A130979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*(46*n-21)*a(n-1) - 896*(2*n-3)*a(n-2))/n with initial
 *            values a(0)=1, a(1)=8. Uses recurrence n*a(n) +
 *            2*(-46*n+21)*a(n-1) + 896*(2*n-3)*a(n-2) = 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t max_computed = (idx_t)-1;

    /* Initialize cache for a(0) and a(1). */
    if (cache == NULL) {
        capacity = 2ull;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        max_computed = 1;
    }

    /* Return cached value if already computed. */
    if ((idx_t)n <= max_computed)
        return cache[n];

    /* Grow cache if necessary. */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* reallocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];

        /* coefficients: 2*(46*i-21) and 896*(2*i-3) */
        seq_t coeff1 = (seq_t)(92 * i - 42);          /* 2*(46*i-21) */
        seq_t coeff2 = (seq_t)(896 * (2 * i - 3));    /* 896*(2*i-3) */

#ifdef __cplusplus
        /* In C++ build seq_t may be a big‑int class; use its operators. */
        seq_t term1 = coeff1 * a_nm1;
        seq_t term2 = coeff2 * a_nm2;
        seq_t num = term1 - term2;
        cache[i] = num / (seq_t)i;
#else
        /* In C build we use unsigned __int128 to avoid overflow during the
           intermediate multiplication.  The final result fits in seq_t. */
        unsigned __int128 t1 = (unsigned __int128)coeff1 * (unsigned __int128)a_nm1;
        unsigned __int128 t2 = (unsigned __int128)coeff2 * (unsigned __int128)a_nm2;
        unsigned __int128 num = t1 - t2;
        cache[i] = (seq_t)(num / i);
#endif
    }
    max_computed = n;
    return cache[n];
}
