/*
 * A170612  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170612
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

/* A170612: Number of reduced words of length n in the Coxeter group
 * on 27 generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 *
 * The generating function is
 *   (t^48 + 2*t^47 + … + 2*t + 1) / (325*t^48 - 25*t^47 - … - 25*t + 1).
 *
 * This yields the linear recurrence
 *
 *   a(0) = 1
 *   for n > 0:
 *       a(n) = Σ_{k=1}^{47} 25 * a(n-k)          // positive terms
 *              - 325 * a(n-48)                    // negative term
 *              + b(n)
 *
 *   b(n) = 1   if n == 0 or n == 48
 *          2   if 1 <= n <= 47
 *          0   otherwise
 *
 * The implementation memoises computed values so subsequent calls
 * are O(1).  All arithmetic uses only the allowed operators.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* memoised values a(0)…a(k) */
static size_t cache_len = 0;      /* current length of cache (next free index) */

seq_t a(idx_t n)
{
    /* expand cache up to index n (inclusive) */
    if ((size_t)n >= cache_len) {
        size_t old_len = cache_len;
        size_t new_len = (size_t)n + 1;

        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* initialise new slots (they will be overwritten anyway) */
        for (size_t i = old_len; i < new_len; ++i)
            cache[i] = 0;

        /* compute required entries */
        for (size_t k = old_len; k <= (size_t)n; ++k) {
            seq_t val = 0;

            /* Σ_{i=1}^{47} 25 * a(k-i)   (skip terms where k-i < 0) */
            for (size_t i = 1; i <= 47 && k >= i; ++i)
                val += (seq_t)25 * cache[k - i];

            /* add b(k) */
            if (k == 0 || k == 48)
                val += (seq_t)1;
            else if (k <= 47)
                val += (seq_t)2;

            /* subtract 325 * a(k-48) when k >= 48 */
            if (k >= 48)
                val -= (seq_t)325 * cache[k - 48];

            cache[k] = val;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
