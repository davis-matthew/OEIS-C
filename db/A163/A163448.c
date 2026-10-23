/*
 * A163448  a(n) = 20*a(n-1) - 98*a(n-2) for n > 1; a(0) = 1, a(1) = 12.
 * https://oeis.org/A163448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1) - 98*a(n-2), a(0)=1, a(1)=12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_N 10000

/* Cached values up to MAX_N.  The array is indexed exactly as the OEIS
   offsets, so a(0) is stored at cache[0], a(1) at cache[1], etc.  */
static seq_t cache[MAX_N + 1];
static idx_t max_cached = 0;        /* largest index whose value is cached   */
static bool initialized = false;   /* whether the cache has been seeded     */

/* Computes a(n) according to the given recurrence, using a simple
   memoisation scheme that is safe for any order of calls. */
seq_t a(idx_t n)
{
    /* Initialise the cached base cases only once. */
    if (!initialized) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        max_cached = 1;
        initialized = true;
    }

    /* If the requested index is within the pre‑allocated range,
       use the cached array and extend it only if necessary. */
    if (n <= (idx_t)MAX_N) {
        if (n > max_cached) {
            for (idx_t i = max_cached + 1; i <= n; ++i) {
                seq_t t1 = cache[i - 1] * (seq_t)20;
                seq_t t2 = cache[i - 2] * (seq_t)98;
                cache[i] = t1 - t2;
            }
            max_cached = n;
        }
        return cache[(size_t)n];
    }

    /* For indices beyond the fixed cache, compute on the fly without
       retaining any intermediate results.  This path is not expected
       to be used in the standard test ranges (n ≈ 0…10000). */
    seq_t a0 = (seq_t)1;         /* a(0) */
    seq_t a1 = (seq_t)12;        /* a(1) */
    if (n == 0) return a0;
    if (n == 1) return a1;

    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * (seq_t)20 - a0 * (seq_t)98;
        a0 = a1;
        a1 = an;
    }
    return an;
}
