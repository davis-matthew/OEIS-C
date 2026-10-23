/*
 * A170601  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2 + 14 * Σ_{k=0}^{n-1} a(k) for 1 ≤ n ≤ 47;
 *            a(n) = 14 * Σ_{k=1}^{47} a(n-k) − 105 * a(n-48) for n ≥ 48.
 *            (These relations follow from the rational generating function of
 *            A170601.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static cache for computed terms */
static seq_t *cache   = NULL;   /* array holding a(0..cache_size-1) */
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap   = 0;   /* allocated array capacity */

/* extend the cache so that it contains a(0) … a(n) */
static void ensure(idx_t n)
{
    if (cache_size > n) return;                    /* already computed */

    size_t needed = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        cache_cap = needed;
    } else if (needed > cache_cap) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache_cap = needed;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i <= 47U) {                    /* first 48 terms */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) sum += cache[k];
            cache[i] = (seq_t)2 + (seq_t)14 * sum;
        } else {                                 /* n ≥ 48 */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 47; ++k) sum += cache[i - k];
            cache[i] = (seq_t)14 * sum - (seq_t)105 * cache[i - 48];
        }
    }
    cache_size = n + 1;
}

/* public function returning a(n) */
seq_t a(idx_t n)
{
    ensure(n);
    return cache[n];
}
