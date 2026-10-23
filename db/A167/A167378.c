/*
 * A167378  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167378 – Number of reduced words of length n in the Coxeter
   group on 31 generators with (S_i)^2 = (S_i S_j)^14 = I.
   Recurrence derived from the rational generating function P(t)/Q(t).

   a(0) = 1.
   For 1 ≤ n ≤ 13:
     a(n) = 2 + 29 * Σ_{k=1}^{n} a(n–k).
   For n ≥ 14:
     a(n) = 1 + 29 * Σ_{k=1}^{13} a(n–k) – 435 * a(n–14).
*/
#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values a(0)…a(N) */
static size_t cache_len = 0;     /* number of cached terms */

seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if ((idx_t)cache_len <= n) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);           /* memory exhausted */
        cache = new_cache;

        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            /* P_i from the numerator polynomial */
            seq_t Pi = 0;
            if (i == 0)           Pi = 1;
            else if (i <= 13)     Pi = 2;
            else if (i == 14)     Pi = 1;
            /* sum of the last 13 (or fewer) preceding terms */
            seq_t sum = 0;
            idx_t up = (i < 13) ? i : 13;
            for (idx_t j = 1; j <= up; ++j) {
                sum += cache[i - j];
            }
            seq_t val = Pi + (seq_t)29 * sum;
            if (i >= 14) {
                val = val - (seq_t)435 * cache[i - 14];
            }
            cache[i] = val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
