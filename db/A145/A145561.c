/*
 * A145561  Alternating row sums of triangle A049029 (S2(5)).
 * https://oeis.org/A145561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = y(n) with offset 1. Initial values: y(0) = -1, y(1) = 1,
 *            y(2) = 4, y(3) = 31, y(4) = 359. For k ≥ 0: -32*k*(1 + k)*(1 + 2
 *            k)*(1 + 4 k)*(3 + 4 k)*y(k) +(1679 + 5920 k + 8080 k² + 5120 k³
 *            + 1280 k⁴)*y(k+1) +(-2550 - 4580 k - 2880 k² - 640 k³)*y(k+2)
 *            +(675 + 640 k + 160 k²)*y(k+3) +(-50 - 20...
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 14, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for previously computed terms. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;    /* number of allocated slots */
static idx_t cache_max  = 0;    /* largest index that has been computed */

/* Ensure cache holds at least 'need+1' slots (indices 0..need). */
static void ensure_cached(idx_t need)
{
    if (cache_size > need) return;
    idx_t new_size = cache_size == 0 ? 8 : cache_size * 2;
    while (new_size <= need) new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();          /* memory exhaustion – stop */
    cache = tmp;
    cache_size = new_size;
}

/* Initialise cache with the first five known values. */
static void init_cache(void)
{
    if (cache_size == 0) {
        ensure_cached(4);       /* indices 0 … 4 */
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)31;
        cache[4] = (seq_t)359;
        cache_max = 4;
    }
}

/* Compute terms up to and including index 'n'. */
static void compute_to(idx_t n)
{
    init_cache();
    if (cache_max >= n) return;
    if (n < 5) { cache_max = n; return; }   /* nothing more to compute */

    ensure_cached(n);          /* make room for indices up to n */

    for (idx_t t = cache_max + 1; t <= n; ++t) {
        /* For t < 5 the value is already known. */
        if (t < 5) continue;

        idx_t k = t - 5;
        seq_t ks = (seq_t)k;

        /* Term for y(k) */
        seq_t term0 = -((seq_t)32 * ks *
                        (1 + ks) *
                        (1 + (seq_t)2 * ks) *
                        (1 + (seq_t)4 * ks) *
                        (3 + (seq_t)4 * ks) *
                        cache[k]);

        /* Coefficient for y(k+1) */
        seq_t coeff1 = (seq_t)1679 + (seq_t)5920 * ks +
                       (seq_t)8080 * ks * ks +
                       (seq_t)5120 * ks * ks * ks +
                       (seq_t)1280 * ks * ks * ks * ks;
        seq_t term1 = coeff1 * cache[k + 1];

        /* Coefficient for y(k+2) */
        seq_t coeff2 = (seq_t)-2550 - (seq_t)4580 * ks -
                       (seq_t)2880 * ks * ks -
                       (seq_t)640  * ks * ks * ks;
        seq_t term2 = coeff2 * cache[k + 2];

        /* Coefficient for y(k+3) */
        seq_t coeff3 = (seq_t)675 + (seq_t)640 * ks +
                       (seq_t)160 * ks * ks;
        seq_t term3 = coeff3 * cache[k + 3];

        /* Coefficient for y(k+4) */
        seq_t coeff4 = (seq_t)-50 - (seq_t)20 * ks;
        seq_t term4 = coeff4 * cache[k + 4];

        seq_t sum = term0 + term1 + term2 + term3 + term4;
        cache[t] = -sum;          /* y(k+5) = -sum */
    }

    cache_max = n;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 1, safety */
    compute_to(n);
    return cache[n];
}
