/*
 * A130915  Number of permutations in the symmetric group S_n in which cycle
 *          lengths are odd and greater than 1.
 * https://oeis.org/A130915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 1,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130915: Number of permutations in S_n with all
   cycle lengths odd and >1.  Recurrence:
   a(0)=1, a(1)=0, a(2)=0
   a(n) = (n-1)*(n-2)*(a(n-2)+a(n-3))  for n>=3
*/
#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* Cache of previously computed terms: indices 0 .. cache_len-1 */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure that cache[0..n] are computed */
static void ensure_cache(idx_t n)
{
    if (cache_len > (size_t)n)
        return;                 /* already computed */

    size_t old_len = cache_len; /* length before growing */
    size_t new_len = (size_t)(n + 1);

    cache = seq_realloc(cache, old_len, new_len);

    /* Initialise base terms if this is the first allocation */
    if (old_len == 0) {
        cache[0] = 1;
        if (new_len > 1) cache[1] = 0;
        if (new_len > 2) cache[2] = 0;
    }

    /* Compute any missing terms */
    for (idx_t i = (idx_t)old_len; i <= n; ++i) {
        if (i < 3) /* already set */ continue;
        seq_t sum  = cache[i-2] + cache[i-3];
        seq_t mult = (seq_t)(i-1) * (seq_t)(i-2);
        cache[i] = mult * sum;
    }

    cache_len = new_len;
}

/* Required entry point */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
