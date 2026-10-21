/*
 * A200999  Triangular numbers, T(m), that are four-thirds of another
 *          triangular number; T(m) such that 3*T(m) = 4*T(k) for some k.
 * https://oeis.org/A200999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 194*a(n-1) - a(n-2) + 28 (n > 1, a(0) = 0, a(1) = 28)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* The maximum index that will be required in the typical tests (∼10 000).  
   The array is static so that constructor calls for an arbitrary-precision
   type are performed automatically. */
#define A200999_MAX_N 10000

static seq_t cache[A200999_MAX_N + 1];

/* Number of cache entries that have been initialized (len >= 2 after
   initialization). */
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Handle the first value directly. */
    if (n == 0)
        return (seq_t)0;

    /* Initialise cache[0] and cache[1] on first use. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)28;
        cache_len = 2;
    }

    /* Expand the cache if necessary. */
    if (n >= cache_len) {
        if (n > A200999_MAX_N) {
            /* The caller requested an index larger than we pre‑allocated.  
               For such cases we can compute the terms on the fly without
               cache; recursion depth is a few iterations, so this is safe. */
            seq_t prev2 = cache[cache_len - 2];
            seq_t prev1 = cache[cache_len - 1];
            for (idx_t i = cache_len; i <= n; ++i) {
                seq_t cur = ((seq_t)194 * prev1) - prev2 + (seq_t)28;
                prev2 = prev1;
                prev1 = cur;
                if (i <= A200999_MAX_N)
                    cache[i] = cur;
            }
            /* Return the computed value. */
            return ((n <= A200999_MAX_N) ? cache[n] : prev1);
        } else {
            /* Compute and store up to n using the recurrence. */
            for (idx_t i = cache_len; i <= n; ++i) {
                cache[i] = ((seq_t)194 * cache[i - 1]) - cache[i - 2] + (seq_t)28;
            }
            cache_len = n + 1;
        }
    }

    return cache[n];
}
