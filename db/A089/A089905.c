/*
 * A089905  Sum of digits of numbers between 0 and (3/9)*(10^n-1).
 * https://oeis.org/A089905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 22*a(n-1) - 141*a(n-2) + 220*a(n-3) - 100*a(n-4), with
 *            a(0)=0, a(1)=6, a(2)=183, a(3)=3285
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure that cache[0..n] is computed.  */
static void
ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);

    /* Initialise base cases if this is the first allocation.  */
    if (cache == NULL) {
        if (new_len > 0)  tmp[0] = 0;
        if (new_len > 1)  tmp[1] = 6;
        if (new_len > 2)  tmp[2] = 183;
        if (new_len > 3)  tmp[3] = 3285;
    } else {
        /* The cache already contains values 0 .. cache_len-1.  */
        /* Ensure base values are present up to index 3.  */
        if (cache_len <= 1 && new_len > 0)  tmp[0] = 0;
        if (cache_len <= 2 && new_len > 1)  tmp[1] = 6;
        if (cache_len <= 3 && new_len > 2)  tmp[2] = 183;
        if (cache_len <= 4 && new_len > 3)  tmp[3] = 3285;
    }

    /* Compute new entries using the recurrence.  */
    idx_t start = cache_len >= 4 ? cache_len : 4;
    for (idx_t i = start; i < new_len; ++i)
        tmp[i] = 22 * tmp[i - 1] - 141 * tmp[i - 2] + 220 * tmp[i - 3] - 100 * tmp[i - 4];

    cache = tmp;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
