/*
 * A174643  Partial sums of A050536.
 * https://oeis.org/A174643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(0) = 8 * a(n) = a(n‑1) + f(n) for n>0 * f(0) = 8 * f(n+1) =
 *            f(n) * (f(n)+1) / 2 (iterated triangular numbers) * Therefore
 *            a(n) = Sum_{i=0..n} A050536(i)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   next_tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/next_tri.h"

/* cache of partial sums */
static seq_t *a_cache = NULL;
/* cache of iterated triangular numbers */
static seq_t *f_cache = NULL;
/* current allocated capacity of the caches */
static idx_t cache_cap = 0;
/* number of terms already computed (cache_len = cache_len of caches) */
static idx_t cache_len = 0;

/* Ensure the caches have at least 'needed' slots */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp;

    tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    a_cache = tmp;

    tmp = (seq_t *)realloc(f_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    f_cache = tmp;

    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (cache_len == 0) {                  /* first call: initialise term 0 */
        ensure_capacity(1);
        f_cache[0] = (seq_t)8;
        a_cache[0] = (seq_t)8;
        cache_len = 1;
    }

    if (n < cache_len)                     /* already computed */
        return a_cache[n];

    ensure_capacity(n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = f_cache[i - 1];
        seq_t cur  = next_tri(prev);
        f_cache[i] = cur;
        a_cache[i] = a_cache[i - 1] + cur;
    }
    cache_len = n + 1;
    return a_cache[n];
}
