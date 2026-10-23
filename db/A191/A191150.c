/*
 * A191150  Hypersigma(n), definition 1: sum of the divisors of n plus the
 *          recursive sum of the divisors of the restricted divisors.
 * https://oeis.org/A191150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1 + Σ_{d|n, d<n} (a(d)+d) (OEIS A191150) * Recurrence:
 *            a(1)=1 * This implementation recomputes all values up to the
 *            maximum * index requested in a single call using a divisor
 *            accumulation sieve. * It uses the provided helper
 *            `alloc_seq_array` for allocating * arrays of the
 *            arbitrary‑prec...
 * Range:     u8 n <= 35, u16 n <= 1439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *aCache   = NULL;   /* a[1..n] */
    static seq_t *sumCache = NULL;   /* accumulated Σ(a(d)+d) for each m */
    static size_t cap = 0;           /* current allocated size (n+1) */

    if (n == 0)            /* sequence is defined for n ≥ 1 */
        return 0;

    /* Allocate or grow the tables if necessary */
    if ((size_t)n + 1 > cap) {
        free(aCache);
        free(sumCache);
        cap = (size_t)n + 1;
        aCache   = alloc_seq_array(cap);
        sumCache = alloc_seq_array(cap);
    }

    /* Zero the divisor‑sum table for the new range */
    for (idx_t i = 1; i <= n; ++i)
        sumCache[i] = (seq_t)0;

    /* Main divisor sieve */
    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1) {
            aCache[1] = 1;
        } else {
            aCache[i] = (seq_t)(i - 1) + sumCache[i];
        }

        /* Value to add to all proper multiples of i */
        seq_t add = aCache[i] + (seq_t)i;

        for (idx_t m = i + i; m <= n; m += i)
            sumCache[m] += add;
    }

    return aCache[n];
}
