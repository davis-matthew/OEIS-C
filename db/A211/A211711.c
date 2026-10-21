/*
 * A211711  Number of (n+1)X(n+1) -11..11 symmetric matrices with every 2X2
 *          subblock having sum zero and three distinct values.
 * https://oeis.org/A211711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 27, u64 n <= 59, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) satisfies the order‑21 linear recurrence
 * a(n) =  6*a(n-1) + 3*a(n-2) - 83*a(n-3) + 92*a(n-4) + 443*a(n-5) 
 *         - 868*a(n-6) - 1073*a(n-7) + 3394*a(n-8) + 748*a(n-9) 
 *         - 7085*a(n-10) + 1842*a(n-11) + 8231*a(n-12) 
 *         - 4528*a(n-13) - 5075*a(n-14) + 3936*a(n-15) + 1424*a(n-16) 
 *         - 1511*a(n-17) - 121*a(n-18) + 244*a(n-19) - 6*a(n-20) - 12*a(n-21)
 * with initial values a(1)…a(21) as listed below.
 */

#include <stddef.h>
#include <stdlib.h>

/* initial 21 terms of the sequence */
static const seq_t init[21] = {
    (seq_t)230,      (seq_t)516,      (seq_t)1030,
    (seq_t)1908,     (seq_t)3528,     (seq_t)6312,
    (seq_t)11390,    (seq_t)20316,    (seq_t)36520,
    (seq_t)65646,    (seq_t)118702,   (seq_t)216006,
    (seq_t)394802,   (seq_t)728492,   (seq_t)1348550,
    (seq_t)2522796,  (seq_t)4730304,  (seq_t)8960110,
    (seq_t)16997998, (seq_t)32539860, (seq_t)62348810
};

/* recurrence coefficients (order 21) */
static const int coeff[21] = {
    6, 3, -83, 92, 443,
    -868, -1073, 3394, 748, -7085,
    1842, 8231, -4528, -5075, 3936,
    1424, -1511, -121, 244, -6, -12
};

/* cache for memoised terms (1‑based index, cache[0] unused) */
static seq_t *cache   = NULL;
static size_t cache_sz = 0;   /* highest index computed so far */

/* provide a minimal doubling allocation helper */
static void ensure_cache_capacity(size_t n)
{
    if (cache == NULL) {
        size_t init_cap = 64;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return;                 /* out of memory – caller ignores */
        for (size_t i = 0; i < init_cap; ++i) cache[i] = 0;
        cache_sz = 0;
    }

    if (n + 1 > cache_sz + 1) {
        size_t new_cap = (cache_sz + 1) * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;                 /* keep old cache if realloc fails */
        cache = tmp;
    }
}

/* populate the cache up to index n (1‑based) */
static void compute_cache_to(size_t n)
{
    if (n <= cache_sz) return;

    ensure_cache_capacity(n);

    /* fill initial 21 terms if they have not yet been written */
    if (cache_sz < 21) {
        for (size_t i = cache_sz + 1; i <= 21 && i <= n; ++i)
            cache[i] = init[i - 1];
        if (n <= 21) { cache_sz = n; return; }
        cache_sz = 21;
    }

    /* iterate from current size+1 up to n */
    for (size_t i = cache_sz + 1; i <= n; ++i) {
        seq_t res = 0;
        for (int k = 0; k < 21; ++k) {
            seq_t term = cache[i - (size_t)(k + 1)];
            /* absolute value of coefficient (positive within 32‑bit range) */
            seq_t mult = term * (seq_t)(unsigned int)(coeff[k] > 0 ?
                           coeff[k] : -coeff[k]);
            if (coeff[k] > 0)
                res += mult;
            else
                res -= mult;
        }
        cache[i] = res;
    }
    cache_sz = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but harmless */

    size_t idx = (size_t)n;
    if (idx <= 21) return init[idx - 1];
    compute_cache_to(idx);
    return cache[idx];
}
