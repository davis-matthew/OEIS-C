/*
 * A080239  Antidiagonal sums of triangle A035317.
 * https://oeis.org/A080239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=1; for n≥3, a(n) = a(n−1)+a(n−2) if n mod 4 ≠ 1;
 *            otherwise a(n) = a(n−1)+a(n−2)+1
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;     /* cached values, index 1 .. N */
static idx_t a_cache_max = 0;     /* maximum n cached so far */
static size_t a_cache_size = 0;   /* allocated array size */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not defined for 0, return 0 */

    /* Ensure the cache is large enough */
    size_t needed = (size_t)n + 1;
    if (needed > a_cache_size) {
        size_t new_size = needed * 2;                 /* double for future growth */
        seq_t *new_mem = (seq_t *)realloc(a_cache,
                                          new_size * sizeof(seq_t));
        if (!new_mem) return (seq_t)0; /* allocation failure */
        /* zero new elements (trivial types assumed) */
        if (new_size > a_cache_size)
            for (size_t i = a_cache_size; i < new_size; ++i)
                new_mem[i] = (seq_t)0;
        a_cache = new_mem;
        a_cache_size = new_size;
    }

    /* Compute missing values up to n */
    for (idx_t i = a_cache_max + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            a_cache[i] = (seq_t)1;
        } else {
            if (i % 4u == 1u)
                a_cache[i] = a_cache[i - 1] + a_cache[i - 2] + (seq_t)1;
            else
                a_cache[i] = a_cache[i - 1] + a_cache[i - 2];
        }
    }
    a_cache_max = n;
    return a_cache[n];
}
