/*
 * A175867  a(0) = 2; a(n) = a(n - 1) * 2 + 1 if a(n - 1) is prime, or a(n -
 *          1) / (smallest prime factor) if it is composite.
 * https://oeis.org/A175867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=2; a(n)=a(n-1)*2+1 if a(n-1) is prime; otherwise
 *            a(n)=a(n-1)/(smallest prime factor of a(n-1))
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   is_prime_big, smallest_prime_factor_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/smallest_prime_factor_seq.h"

static seq_t *cache = NULL;      /* computed terms */
static idx_t cache_size = 0;     /* number of terms stored */
static size_t cache_cap  = 0;    /* allocated capacity */

/* Extend cache up to at least max_n (inclusive). */
static void ensure_cache(idx_t max_n)
{
    if (max_n < cache_size)
        return;

    size_t new_cap = (size_t)(max_n + 1);

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = new_cap;
        cache[0] = (seq_t)2;
        cache_size = 1;
    } else if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_size; i <= max_n; ++i) {
        seq_t prev = cache[i - 1];
        if (is_prime_big(prev)) {
            cache[i] = prev * (seq_t)2 + (seq_t)1;
        } else {
            seq_t spf = smallest_prime_factor_seq(prev);
            cache[i] = prev / spf;
        }
    }
    cache_size = (idx_t)(max_n + 1);
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
