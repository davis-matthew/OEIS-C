/*
 * A038197  4-wave sequence.
 * https://oeis.org/A038197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence from OEIS) :
   a(0)=1, a(1)=1, a(2)=1, a(3)=1
   For n≥4:
     if n≡1 (mod 3) : a(n) = a(n-1)+a(n-2)
     if n≡2 (mod 3) : a(n) = a(n-1)+a(n-4)
     if n≡0 (mod 3) : a(n) = a(n-1)+a(n-6)
   Implements a cache to allow random‑order calls. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* cache array for computed values */
static idx_t cache_size = 0;  /* number of entries currently computed (cache_size==0 => none) */

seq_t a(idx_t n)
{
    /* Ensure the cache has room for index n */
    if (n >= cache_size) {
        size_t new_cap = cache_size ? cache_size * 2 : 16;
        if (new_cap <= (size_t)n) {
            /* grow until we can store n */
            while (new_cap <= (size_t)n) new_cap <<= 1;
        }
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* initialise new entries to 0 (not strictly necessary) */
        for (size_t i = cache_size; i < new_cap; ++i) cache[i] = 0;
        cache_size = new_cap;
    }

    /* Fill in values up to n (only if not already done) */
    for (idx_t i = 0; i <= n; ++i) {
        if (i < cache_size && cache[i] != 0) continue;  /* already computed */
        if (i == 0 || i == 1 || i == 2 || i == 3) {
            cache[i] = 1;
        } else {
            if (i % 3 == 1) {          /* n = 3*m + 1 */
                cache[i] = cache[i-1] + cache[i-2];
            } else if (i % 3 == 2) {   /* n = 3*m + 2 */
                cache[i] = cache[i-1] + cache[i-4];
            } else {                    /* n = 3*m   */
                cache[i] = cache[i-1] + cache[i-6];
            }
        }
    }

    return cache[n];
}
