/*
 * A045499  Fourth-from-right diagonal of triangle A121207.
 * https://oeis.org/A045499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(n+1) = Σ_{k=0}^{n} binomial(n+3, k+3) * a(k) (OEIS
 *            A045499)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache   = NULL;
    static idx_t max_idx  = 0;   /* maximum index currently stored (inclusive) */
    static size_t cap     = 0;   /* allocated capacity */

    /* Ensure capacity for index n */
    if (cap < n + 1) {
        size_t newcap = cap ? cap : 1;
        while (newcap < n + 1) newcap <<= 1;
        cache = seq_realloc(cache, cap, newcap);
        cap   = newcap;
    }

    /* Initialise a(0) if necessary */
    if (max_idx == 0) {
        cache[0] = 1;
        max_idx = 0;
    }

    /* Compute missing values up to n */
    for (idx_t i = max_idx + 1; i <= n; ++i) {
        /* recurrence for a(i) : i >= 1 */
        /* a(i) = Σ_{k=0}^{i-1} binom(i+2, k+2) * a(k)     */
        seq_t sum = 0;
        /* starting binomial coefficient binom(i+2,3) */
        seq_t b = ((seq_t)(i + 2) * (i + 1) * i) / 6;
        for (idx_t k = 0; k < i; ++k) {
            sum += b * cache[k];
            if (k < i - 1) {
                /* update b to binom(i+2, k+4) from binom(i+2, k+3) */
                b = b * (i - k - 1) / (k + 4);
            }
        }
        cache[i] = sum;
    }
    max_idx = n;

    return cache[n];
}
