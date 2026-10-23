/*
 * A170288  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 <= n <= 40: a(n) = 2 + 37 * sum_{k=0}^{n-1} a(k)
 *            For n >= 41: a(n) = 37 * (a(n-1)+...+a(n-40)) - 703 * a(n-41)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size)
        return;

    if (cache_cap < needed) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 64;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();          /* memory allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    for (size_t i = cache_size; i < needed; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else if (i <= 40) {
            seq_t sum = 0;
            for (size_t k = 0; k < i; ++k)
                sum += cache[k];
            cache[i] = 2 + 37 * sum;
        } else {
            seq_t sum40 = 0;
            for (size_t k = i - 40; k < i; ++k)
                sum40 += cache[k];
            cache[i] = 37 * sum40 - 703 * cache[i - 41];
        }
    }
    cache_size = needed;
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[(size_t)n];
}
