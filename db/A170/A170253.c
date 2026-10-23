/*
 * A170253  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 4 for 2 ≤ n ≤ 40: a(n) = 3 * a(n−1) a(41) = 2 *
 *            Σ_{k=1}^{40} a(41−k) − 2 for n ≥ 42: a(n) = 2 * Σ_{k=1}^{40}
 *            a(n−k) − 3 * a(n−41)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_last = 0;
static size_t cache_cap  = 0;

static void
ensure_cache_capacity(idx_t need)
{
    if (need < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= (size_t)need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        ensure_cache_capacity(0);
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_last = 0;
    }

    /* expand cache up to n if necessary */
    if (n > cache_last) {
        ensure_cache_capacity(n);
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else {
                seq_t sum = 0;
                for (idx_t k = 1; k <= 40; ++k) {
                    if (k <= i)
                        sum += cache[i - k];
                }
                if (i <= 40) {
                    val = 2 * sum + 2;
                } else if (i == 41) {
                    val = 2 * sum - 2;
                } else { /* i ≥ 42 */
                    val = 2 * sum - 3 * cache[i - 41];
                }
            }
            cache[i] = val;
        }
        cache_last = n;
    }

    return cache[n];
}
