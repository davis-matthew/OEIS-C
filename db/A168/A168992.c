/*
 * A168992  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(0) = 1 a(n) = 27 * 26^(n-1) for 1 ≤ n < 23 a(n) = 25 *
 *            Σ_{i=1}^{22} a(n-i) - 325 * a(n-23) for n ≥ 23
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"

static seq_t *cache = NULL;      /* memoised values */
static idx_t  cache_len = 0;     /* how many values are stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* Ensure the cache can hold indices up to n. */
static void
ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                             /* allocation failed; abort */
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Return cached value if already known. */
    if (n < cache_len)
        return cache[n];

    /* Expand cache to at least n+1 elements. */
    idx_t new_len = n + 1;
    ensure_capacity(n);

    for (idx_t k = cache_len; k < new_len; ++k) {
        seq_t term;
        if (k == 0) {
            term = (seq_t)1;
        } else if (k < 23) {
            /* a(k) = 27 * 26^(k-1) */
            seq_t powv = seq_pow((seq_t)26, k - 1);
            term = (seq_t)27 * powv;
        } else {
            /* a(k) = 25 * Σ_{i=1}^{22} a(k-i) - 325 * a(k-23) */
            seq_t sum = 0;
            for (idx_t i = 1; i <= 22; ++i)
                sum += cache[k - i];
            term = (seq_t)25 * sum - (seq_t)325 * cache[k - 23];
        }
        cache[k] = term;
    }
    cache_len = new_len;
    return cache[n];
}
