/*
 * A015098  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=-3.
 * https://oeis.org/A015098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n+1) = Σ{i=0..n} (-3)^i * a(i) * a(n-i), a(0) = 1
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A015098 – Carlitz‑Riordan q‑Catalan numbers (recurrence version) with q = –3 */

#include <stdlib.h>

/* static memoization tables */
static seq_t *cache      = NULL;   /* a(0) … a(k)   */
static size_t cache_cap  = 0;       /* allocated capacity of cache   */
static idx_t  cache_sz   = 0;       /* number of values stored       */

static seq_t *pow_neg3   = NULL;   /* (-3)^i for i = 0 … m-1         */
static size_t pow_cap    = 0;       /* allocated capacity of pow_neg3 */

/* grow an array to fit at least <need> elements */
static void grow_arr(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = *cap ? (*cap) * 2 : 16;
        if (new_cap < need)   new_cap = need;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* make sure that a(0)…a(n) are known and pow_neg3[0…n-1] are known */
static void ensure_cache(idx_t n)
{
    if (n < cache_sz) return;               /* already computed   */

    /* enlarge storage as necessary */
    grow_arr(&cache,   &cache_cap, (size_t)(n + 1));
    grow_arr(&pow_neg3,&pow_cap , (size_t)(n + 1));

    if (cache_sz == 0) {                    /* initialise base   */
        cache[0]   = (seq_t)1;
        pow_neg3[0] = (seq_t)1;
        cache_sz = 1;
        /* pow_neg3[0] corresponds to (-3)^0 */
        if (pow_cap > 1) pow_neg3[1] = (seq_t)(-3);
    }

    /* compute terms up to n */
    for (idx_t m = cache_sz; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t i = 0; i <= m - 1; ++i) {
            seq_t prod = pow_neg3[i];
            prod *= cache[i];
            prod *= cache[m - 1 - i];
            sum += prod;
        }
        cache[m] = sum;

        /* store the next power of –3 for future use */
        if (m < pow_cap) {
            pow_neg3[m] = pow_neg3[m - 1] * (seq_t)(-3);
        }
    }
    cache_sz = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
