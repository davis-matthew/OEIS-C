/*
 * A041699  Denominators of continued fraction convergents to sqrt(369).
 * https://oeis.org/A041699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 16793602*a(n-12) - a(n-24) (n > 23), with a(0) … a(23)
 *            the denominators of successive convergents of the continued
 *            fraction expansion of √369
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static size_t  cache_cap = 0;
static size_t  cache_len = 0;

static const idx_t periodic_q[12] = {4,1,3,2,7,4,7,2,3,1,4,38};

static idx_t partial_quotient(idx_t k)
{
    if (k == 0) return (idx_t)19;           /* a0 = ⌊√369⌋ = 19 */
    return periodic_q[(k-1) % 12];           /* periodic part of CF */
}

static void ensure_cache_size(idx_t need)
{
    if ((size_t)need < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 32;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    seq_t *newp;
#ifdef __cplusplus
    newp = new seq_t[new_cap];
#else
    newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!newp) abort();
    for (size_t i = 0; i < cache_len; ++i) newp[i] = cache[i];
    if (cache) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }
    cache = newp;
    cache_cap = new_cap;
}

static void compute_to(idx_t n)
{
    if (cache_len == 0) {                    /* initialise first term */
        ensure_cache_size(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if ((idx_t)n < cache_len) return;
    ensure_cache_size(n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        idx_t a_k = partial_quotient(i);
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            /* q(-1) = 0, q(0) = 1 */
            val = (seq_t)a_k * cache[0];
        } else {
            val = (seq_t)a_k * cache[i-1] + cache[i-2];
        }
        cache[i] = val;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    compute_to(n);
    return cache[n];
}
