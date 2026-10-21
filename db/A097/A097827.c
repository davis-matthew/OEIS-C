/*
 * A097827  Partial sums of Chebyshev sequence S(n,12) = U(n,6) = A004191(n).
 * https://oeis.org/A097827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - a(n-2) + 1, with a(0)=1, a(1)=13
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t max_computed = 0;

static void extend_cache(idx_t n)
{
    size_t required = (size_t)(n + 1);
    if (required <= cache_size) return;

    size_t new_size = cache_size ? cache_size : 2;
    while (new_size < required) new_size <<= 1;

    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)13;

    extend_cache(n);

    if (max_computed < 1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)13;
        max_computed = 1;
    }

    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)12 * cache[i - 1] - cache[i - 2] + (seq_t)1;
        }
        max_computed = n;
    }

    return cache[n];
}
