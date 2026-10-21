/*
 * A098395  a(n) is the next larger integer including the highest digit of
 *          a(n-1), a(1) = 1.
 * https://oeis.org/A098395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; a(n) = the smallest integer > a(n-1) that contains the *
 *            largest decimal digit of a(n-1). This implements the recursive
 *            definition * of OEIS A098395. Complexity is O(n), because the
 *            gap between successive * terms is at most 10
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   contains_digit_seq_t, largest_digit_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/contains_digit_seq_t.h"
#include "../utils/largest_digit_seq_t.h"

/* ------------------------------------------------------------
 * Cached sequence storage.  a(1) is stored at cache[0].
 * ------------------------------------------------------------
 */
static seq_t *cache = NULL;   /* pointer to cached terms */
static idx_t cache_len = 0;   /* number of terms stored (1‑based index) */

/* ------------------------------------------------------------
 * Main function implementing the sequence.
 * ------------------------------------------------------------
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, but defined for completeness */

    /* initialise cache with a(1) if necessary */
    if (cache_len == 0)
    {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;     /* a(1) = 1 */
        cache_len = 1;
    }

    /* fill cache up to n (indices are 1‑based) */
    for (idx_t i = cache_len; i < n; ++i)
    {
        seq_t prev = cache[cache_len - 1];
        int d = largest_digit_seq_t(prev);

        seq_t cand = prev + (seq_t)1;
        while (!contains_digit_seq_t(cand, d))
            cand += (seq_t)1;

        /* grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache[cache_len] = cand;
        ++cache_len;
    }

    return cache[n - 1];
}
