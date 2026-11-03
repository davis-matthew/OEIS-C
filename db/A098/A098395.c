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
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------
 * Helper: does a seq_t value contain a particular decimal digit.
 * ------------------------------------------------------------
 */
static int contains_digit_seq_t(seq_t v, int d)
{
    seq_t x = v;
    if (x == 0) return d == 0;
    while (x > 0)
    {
        if ((int)(x % 10) == d) return 1;
        x /= 10;
    }
    return 0;
}

/* ------------------------------------------------------------
 * Helper: largest decimal digit in a seq_t value.
 * ------------------------------------------------------------
 */
static int largest_digit_seq_t(seq_t v)
{
    int max = 0;
    seq_t x = v;
    while (x > 0)
    {
        int d = (int)(x % 10);
        if (d > max) max = d;
        x /= 10;
    }
    return max;
}

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
        cache = mem_new(seq_t, 1);
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
        seq_t *tmp = mem_resize(seq_t, cache, (cache_len + 1));
        if (!tmp) abort();
        cache = tmp;
        cache[cache_len] = cand;
        ++cache_len;
    }

    return cache[n - 1];
}
