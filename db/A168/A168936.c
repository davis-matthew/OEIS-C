/*
 * A168936  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A168936: Number of reduced words of length n in a Coxeter group on 19 generators
 * with relations S_i^2 = I and (S_i S_j)^22 = I.
 *
 * The generating function is
 *   G(t) = (t^22 + 2*t^21 + ... + 2*t + 1) /
 *          (1 - 17*t - 17*t^2 - ... - 17*t^21 + 153*t^22)
 *
 * From this we obtain the linear recurrence (for all n ≥ 0)
 *
 *   a(n) = 17*a(n-1) + 17*a(n-2) + ... + 17*a(n-21)
 *           - 153*a(n-22) + p(n)
 *
 * where the inhomogeneous term p(n) is
 *   p(0) = 1,   p(22) = 1,
 *   p(n) = 2 for 1 ≤ n ≤ 21,
 *   p(n) = 0 for n > 22.
 *
 * The function below memoises computed values so that a(n) can be requested
 * in any order without recomputation.  It works for any of the supported
 * seq_t types (uint8_t … unsigned __int128 or an arbitrary‑precision class).
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure capacity for indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If we already have the value, return it */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Constants used in the recurrence */
    const seq_t C17  = 17;
    const seq_t C153 = 153;

    /* Compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;

        /* Sum 17*a[i-k] for k = 1 .. min(21, i) */
        idx_t maxk = (i < 21) ? i : 21;
        for (idx_t k = 1; k <= maxk; ++k)
            sum += C17 * cache[(size_t)(i - k)];

        /* Subtract 153*a[i-22] when i >= 22 */
        if (i >= 22)
            sum -= C153 * cache[(size_t)(i - 22)];

        /* Add the inhomogeneous term p(i) */
        seq_t p = 0;
        if (i == 0 || i == 22)            p = 1;
        else if (i >= 1 && i <= 21)       p = 2;
        /* otherwise p == 0 */

        cache[(size_t)i] = sum + p;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
