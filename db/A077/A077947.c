/*
 * A077947  Expansion of 1/(1 - x - x^2 - 2*x^3).
 * https://oeis.org/A077947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3); with a(0) = 1, a(1) = 1, a(2)
 *            = 2 (OEIS A077947)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;

seq_t a(idx_t n)
{
    /* Return cached value if already computed   */
    if ((size_t)n < a_cache_len)
        return a_cache[n];

    /* Grow the cache to accommodate index n   */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    if (!tmp) abort();           /* OOM, terminate program */
    a_cache = tmp;

    /* Initialise base values if needed      */
    if (new_len > 0) a_cache[0] = (seq_t)1;
    if (new_len > 1) a_cache[1] = (seq_t)1;
    if (new_len > 2) a_cache[2] = (seq_t)2;

    /* Compute new entries using the recurrence */
    size_t start = a_cache_len > 3 ? a_cache_len : 3;
    for (size_t i = start; i < new_len; ++i)
        a_cache[i] = a_cache[i-1] + a_cache[i-2] + (a_cache[i-3] << 1);

    a_cache_len = new_len;
    return a_cache[n];
}
