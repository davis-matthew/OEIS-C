/*
 * A170613  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A170613: Number of reduced words of length n in a Coxeter group
 * on 28 generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 *
 * The generating function is
 *   P(t) = 1 + 2t + ... + 2t^47 + t^48
 *   Q(t) = 1 - 26t - 26t^2 - ... - 26t^47 + 351t^48
 *
 * Hence the coefficients a(n) satisfy the linear recurrence
 * for every n ≥ 0:
 *
 *   a(n) = p[n] + 26 ⋅ Σ_{k=1}^{min(n,47)} a(n-k)
 *          − (n ≥ 48 ? 351 ⋅ a(n-48) : 0),
 *
 * where
 *   p[0] = 1,
 *   p[n] = 2 for 1 ≤ n ≤ 47,
 *   p[48] = 1,
 *   p[n] = 0 for n > 48.
 *
 * The implementation caches computed values to allow calls in any order.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"  /* provides seq_alloc(size_t) */

/* Memoised computation of a(n) */
seq_t a(idx_t n)
{
    /* Cached sequence values and bookkeeping */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of values stored  (next free index) */

    /* Convert idx_t to native type suitable for indexing */
    size_t target = (size_t)n;

    /* Ensure capacity for all terms up to target */
    if (target + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap <= target) new_cap <<= 1;

        seq_t *new_cache = seq_alloc(new_cap);
        if (cache) {
            for (size_t i = 0; i < len; ++i)
                new_cache[i] = cache[i];
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute missing terms up to target */
    if (len <= target) {
        /* Maintain the sum of the last 47 terms:
         * sum47 = a[n-47] + ... + a[n-1]   (empty when n=0) */
        seq_t sum47 = 0;
        /* If we have already computed earlier terms, reconstruct sum47 */
        if (len > 0) {
            /* Compute sum47 for the current length */
            size_t start = (len >= 47) ? len - 47 : 0;
            for (size_t i = start; i < len; ++i)
                sum47 += cache[i];
        }

        /* Generate terms from current length to target */
        for (size_t i = len; i <= target; ++i) {
            /* Determine p[i] */
            seq_t p = 0;
            if (i == 0)          p = 1;
            else if (i <= 47)    p = 2;
            else if (i == 48)    p = 1;

            seq_t term = p + 26 * sum47;

            if (i >= 48)
                term -= 351 * cache[i - 48];

            cache[i] = term;

            /* Update sum47 for the next iteration */
            if (i >= 47)             /* window size now 47 */
                sum47 += cache[i] - cache[i - 47];
            else
                sum47 += cache[i];
        }
        len = target + 1;
    }

    return cache[target];
}
