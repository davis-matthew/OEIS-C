/*
 * A170023  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function
   (t^36 + 2*t^35 + … + 2*t + 1) /
   (1 - 12*t - 12*t^2 - … - 12*t^35 + 78*t^36).
   Let p(n) = 1 for n=0 or n=36, 2 for 1≤n≤35, and 0 otherwise.
   Then for all n≥0:
       a(n) = p(n) + 12*∑_{k=1}^{min(n,35)} a(n-k) – 78*a(n-36)   (the
       last term is omitted when n<36).  This satisfies a(0)=1 and
       reproduces all listed values.  The function memoises the values
       so that subsequent calls are O(1).  It grows a static cache
       as needed up to the largest n requested. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoised sequence values */
static idx_t  cache_len = 0;         /* number of elements currently stored */

/* Compute a(x) for any n ≥ 0 */
seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *p = NULL;

        /* Allocate or grow the cache */
        if (cache == NULL) {
            p = (seq_t *)malloc(sizeof(seq_t) * new_len);
            if (!p) abort();
            cache = p;
        } else if (new_len > cache_len) {
            p = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
            if (!p) abort();
            cache = p;
        }

        /* Compute missing terms */
        idx_t old_len = cache_len;
        for (idx_t i = old_len; i <= n; ++i) {
            /* Determine the constant term p(i) */
            seq_t pi;
            if (i == 0)      pi = 1;
            else if (i <= 35) pi = 2;
            else if (i == 36) pi = 1;
            else             pi = 0;

            /* Sum the previous up to 35 terms */
            seq_t sum = 0;
            idx_t start = (i >= 35) ? i - 35 : 0;
            for (idx_t j = start; j < i; ++j) {
                sum += cache[j];
            }

            /* Apply recurrence */
            seq_t val = sum * (seq_t)12;
            if (i >= 36) {
                val -= cache[i - 36] * (seq_t)78;
            }
            val += pi;

            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
