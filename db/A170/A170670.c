/*
 * A170670  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   N_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A170670
 *  Formula: a(0) = 1, a(n) = 35*(a(n-1)+a(n-2)+...+a(n-48)) - 630*a(n-49)  (n ≥ 1)
 *  (With a(n)=37*36^(n-1) for n≤48, the recurrence automatically reproduces that)
 *
 *  This routine caches computed terms in a static array up to n=10000 for speed.
 *  It works for any seq_t/idx_t type defined by the build environment.
 */

#include <stddef.h>   /* for size_t, idx_t etc. */
#include "../utils/N_value.h"

static seq_t cache[10001];
static idx_t max_cached = 0;

seq_t a(idx_t n)
{
    /* Initialize cache[0] once */
    if (max_cached == 0 && n >= 0) {
        cache[0] = (seq_t)1;
        max_cached = 0;
    }

    /* Return cached result if available */
    if (n <= max_cached)
        return cache[n];

    /* Compute terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t sum48 = (seq_t)0;
        for (idx_t j = 1; j <= 48 && i >= j; ++j)
            sum48 += cache[i - j];

        seq_t term49 = (i >= 49) ? cache[i - 49] : (seq_t)0;
        seq_t N = N_value(i);

        cache[i] = N + (seq_t)35 * sum48 - (seq_t)630 * term49;
    }
    max_cached = n;
    return cache[n];
}
