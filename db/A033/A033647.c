/*
 * A033647  Base 10 digital convolution sequence.
 * https://oeis.org/A033647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base 10 digital convolution sequence:
   a(0) = 1.  For n > 0 let n = d_k … d_0 be its decimal digits, then
   a(n) = Σ_i d_i * a(n - i - 1).  */

#include <stdlib.h>

static seq_t *cache = NULL;      /* stored values a(0)…a(cache_len-1) */
static size_t cache_len = 0;     /* number of stored values          */
static size_t cache_cap = 0;     /* allocated capacity                 */

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p)                     /* allocation failed – just bail */
        return;
    cache = p;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            return (seq_t)0;     /* unlikely – out of memory */
        cache[0] = (seq_t)1;
        cache_len = 1;
        cache_cap = 1;
    }

    if (n < cache_len)
        return cache[n];

    /* extend cache up to n */
    ensure_capacity((size_t)n + 1);

    for (idx_t i = cache_len; i <= n; ++i) {
        idx_t tmp = i;
        seq_t sum = (seq_t)0;
        size_t offset = 1;           /* refers to a(i - offset) */

        while (tmp > 0) {
            unsigned digit = tmp % 10;
            tmp /= 10;
            sum += cache[i - offset] * (seq_t)digit;
            ++offset;
        }
        cache[i] = sum;
    }
    cache_len = n + 1;
    return cache[n];
}
