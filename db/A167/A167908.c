/*
 * A167908  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n)=10*9^(n-1) for 1≤n≤15 ;
 *            a(16)=8*∑_{j=1}^{15}a(16-j)-36*a(0)+1 ;
 *            a(n)=8*∑_{j=1}^{15}a(n-j)-36*a(n-16) for n≥17
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values          */
static size_t cache_size = 0;    /* number of terms currently stored  */
static size_t cache_cap  = 0;    /* allocated capacity                 */

static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);          /* out of memory */
    cache = new_ptr;
    cache_cap = new_cap;

    /* initialise first 16 terms only once */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        seq_t t = (seq_t)10;                    /* a(1)   */
        for (idx_t i = 1; i <= 15; ++i) {
            cache[i] = t;
            t = t * (seq_t)9;
        }
        cache_size = 16;                         /* indices 0..15 stored */
    }
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);

    /* If n <= 15 return cached value immediately. */
    if (n < 16) return cache[(size_t)n];

    /* Compute a(16) once if not yet computed. */
    if (cache_size == 16) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= 15; ++j) sum += cache[16 - j];
        cache[16] = (seq_t)8 * sum - (seq_t)36 * cache[0] + (seq_t)1;
        cache_size = 17;                 /* indices 0..16 stored */
    }

    /* Compute any missing terms up to n. */
    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= 15; ++j) sum += cache[k - j];
        cache[k] = (seq_t)8 * sum - (seq_t)36 * cache[k - 16];
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
