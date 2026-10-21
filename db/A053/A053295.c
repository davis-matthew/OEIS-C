/*
 * A053295  Partial sums of A053739.
 * https://oeis.org/A053295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 34, u64 n <= 80, u128 n <= 173,
 *            big any n
 * Helpers:   binomial, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   This implementation uses the recurrence
   a(n) = a(n-1) + a(n-2) + C(n+5,5)  for n ≥ 0,
   with a(0) = 1 and a(1) = 7.
   The binomial coefficient is computed via the supplied
   binomial(idx_t a, idx_t b) routine.
   Results are cached in a static array so that calls in
   any order remain efficient (O(n) time for the first
   computation of each new term).  All arithmetic is
   performed in seq_t, the arbitrary‑precision or fixed‑width
   type chosen by the build system.
*/
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cached_cap = 0;

    /* initialise with the first two terms */
    if (cached_cap == 0) {
        cached_cap = 2;                      /* terms 0 and 1 */
        cache = seq_realloc(NULL, 0, cached_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
    }

    /* extend cache if necessary */
    if ((size_t)n + 1 > cached_cap) {
        size_t new_cap = (size_t)n + 1;
        cache = seq_realloc(cache, cached_cap, new_cap);
        for (idx_t i = (idx_t)cached_cap; i <= n; ++i) {
            seq_t sum = cache[i-1] + cache[i-2];
            seq_t add = binomial(i + (idx_t)5, (idx_t)5);
            cache[i] = sum + add;
        }
        cached_cap = new_cap;
    }

    return cache[n];
}
