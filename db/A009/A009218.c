/*
 * A009218  Expansion of e.g.f. exp(sinh(log(1+x))).
 * https://oeis.org/A009218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A009218: Expansion of e.g.f. exp(sinh(log(1+x))).
   Recurrence for n > 2:
   a(n) = (-2*n+3)*a(n-1) - (n-3)*(n-1)*a(n-2) + (n-1)*(n-2)*a(n-3)/2
   Initial values: a(0)=1, a(1)=1, a(2)=0 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Ensure cache large enough */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;          /* indices 0 … n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – fall back to recomputing on the fly */
            seq_t prev2 = (seq_t)0, prev1 = (seq_t)1, prev0 = (seq_t)1; /* a(0), a(1), a(2) */
            if (n == 3) return (seq_t)1;
            for (idx_t k = 3; k <= n; ++k) {
                seq_t a1 = prev1; /* a(k-1) */
                seq_t a2 = prev0; /* a(k-2) */
                seq_t a3 = prev2; /* a(k-3) */
                seq_t kk = (seq_t)k;
                seq_t term1 = ((seq_t)(-2 * kk + 3)) * a1;
                seq_t term2 = ((seq_t)(kk - 3)) * (kk - 1) * a2;
                seq_t term3 = ((seq_t)(kk - 1)) * (kk - 2) * a3 / 2;
                seq_t cur = term1 - term2 + term3;
                prev2 = prev1;
                prev1 = prev0;
                prev0 = cur;
            }
            return prev0;
        }
        cache = new_cache;
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0) { cache[i] = (seq_t)1; }
            else if (i == 1) { cache[i] = (seq_t)1; }
            else if (i == 2) { cache[i] = (seq_t)0; }
            else {
                seq_t a1 = cache[i - 1];
                seq_t a2 = cache[i - 2];
                seq_t a3 = cache[i - 3];
                seq_t ii = (seq_t)i;
                seq_t term1 = ((seq_t)(-2 * ii + 3)) * a1;
                seq_t term2 = ((seq_t)(ii - 3)) * (ii - 1) * a2;
                seq_t term3 = ((seq_t)(ii - 1)) * (ii - 2) * a3 / 2;
                cache[i] = term1 - term2 + term3;
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
