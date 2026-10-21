/*
 * A088127  E.g.f. exp(-x)*cosh(x)/(1-x)^2.
 * https://oeis.org/A088127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(n-1)*a(n-1) - (n-4)*(n-1)*a(n-2) -
 *            2*(n-2)*(n-1)*a(n-3), with a(0)=1, a(1)=1, a(2)=4 (OEIS A088127)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        return; /* allocation failed – arm‑the caller handles this */;
    cache = new_cache;

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else if (i == 1) {
            cache[i] = 1;
        } else if (i == 2) {
            cache[i] = 4;
        } else if (i == 3) {
            cache[3] = 14;                /* special case because (i-4) = -1 */
        } else {
            seq_t an1 = cache[i - 1];
            seq_t an2 = cache[i - 2];
            seq_t an3 = cache[i - 3];

            seq_t t1 = ((seq_t)2 * (seq_t)(i - 1)) * an1;
            seq_t t2 = ((seq_t)(i - 4) * (seq_t)(i - 1)) * an2;
            seq_t t3 = ((seq_t)2 * (seq_t)(i - 2) * (seq_t)(i - 1)) * an3;

            cache[i] = t1 - t2 - t3;
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
