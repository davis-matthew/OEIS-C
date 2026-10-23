/*
 * A169569  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence derived from the generating function:
 *   G(t) = (t^35 + 2*t^34 + ... + 2*t + 1) / (351*t^35 - 26*t^34 - ... - 26*t + 1)
 *
 * Let N(t) = t^35 + 2 t^34 + ... + 2 t + 1 and D(t) = 351 t^35 - 26 t^34 - ... - 26 t + 1.
 * Then D(t) * G(t) = N(t) gives for every n ≥ 0
 *
 *     a(n) - 26·a(n-1) - ... - 26·a(n-34) + 351·a(n-35) = rhs(n)
 *
 * where rhs(0)=rhs(35)=1, rhs(n)=2 for 1≤n≤34, and rhs(n)=0 otherwise.
 *
 * This is equivalent to the iterative form
 *
 *     a(n) = 26 · Σ_{j=1}^{min(n,34)} a(n-j) - (n≥35 ? 351·a(n-35) : 0) + rhs(n).
 *
 * The function below computes a(n) for any n ≥ 0 using memoisation and runs in O(n·34)
 * time for the first call to n and amortised O(1) for later calls that request larger n.
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? required : cache_cap * 2;
        if (new_cap < required) new_cap = required;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = newp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if needed */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* rhs(n) */
        seq_t rhs;
        if (i == 0 || i == 35) {
            rhs = (seq_t)1;
        } else if (i < 35) {
            rhs = (seq_t)2;
        } else {
            rhs = (seq_t)0;
        }

        /* Σ_{j=1}^{min(i,34)} a(i-j) */
        seq_t sum34 = (seq_t)0;
        for (size_t j = 1; j <= 34 && i >= j; ++j) {
            sum34 += cache[i - j];
        }

        /* temp = 26 * sum34 + rhs   (no intermediate negative values) */
        seq_t temp = (seq_t)26 * sum34 + rhs;

        if (i >= 35) {
            /* a[i] = temp - 351 * a[i-35] */
            cache[i] = temp - (seq_t)351 * cache[i - 35];
        } else {
            cache[i] = temp;
        }
    }
    cache_len = required;
    return cache[(size_t)n];
}
