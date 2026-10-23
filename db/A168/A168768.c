/*
 * A168768  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A168768: Number of reduced words of length n in a Coxeter group on 43 generators
   with relations (S_i)^2 = (S_i S_j)^18 = I.
   The sequence satisfies the linear recurrence derived from the rational
   generating function

      A(t) = (t^18 + 2*t^17 + 2*t^16 + ... + 2*t + 1) /
             ( 1 - 41*t - 41*t^2 - ... - 41*t^17 + 861*t^18 ).

   For n ≥ 18:
      a(n) = 41*(a(n-1) + ... + a(n-17)) - 861*a(n-18)

   For 0 ≤ n ≤ 17, the same relation holds with an extra inhomogeneous term
   p_n given by the numerator:
      p_0 = 1,   p_1 … p_17 = 2,   p_18 = 1,   p_n = 0 for n > 18

   Thus for all n ≥ 0
      a(n) = 41*Σ_{i=1}^{17} a(n-i) – 861*a(n-18) + p_n .
*/
#include <stdlib.h>
#include <stdint.h>

/* static cache for memoised values up to the largest requested index */
static seq_t *cache      = NULL;   /* array of computed values */
static size_t cached_len = 0;      /* number of values currently stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* helper to enlarge the cache to hold at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (need < cache_cap) return;
    size_t new_cap = (cache_cap < 16) ? 16 : cache_cap * 2;
    while (new_cap <= need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* in a real program you would handle out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* main function required by the test harness */
seq_t a(idx_t n)
{
    /* make sure we have computed values up to n */
    if ((size_t)n >= cached_len) {
        ensure_cache((size_t)n + 1);
        /* compute missing terms */
        for (size_t i = cached_len; i <= (size_t)n; ++i) {
            seq_t sum = 0;
            /* sum 41*a[i-1] .. 41*a[i-17] */
            for (size_t j = 1; j <= 17; ++j) {
                if (i >= j) sum += (seq_t)41 * cache[i - j];
            }
            /* subtract 861*a[i-18] if available */
            if (i >= 18) sum -= (seq_t)861 * cache[i - 18];
            /* add inhomogeneous term for first 19 indices */
            if (i == 0 || i == 18) sum += 1;
            else if (i <= 17) sum += 2;
            /* store result */
            cache[i] = sum;
        }
        cached_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
