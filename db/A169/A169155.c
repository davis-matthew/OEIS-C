/*
 * A169155  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A169155 a(0) = 1 a(1) = 46 a(n) = 45 * a(n-1) for 2 ≤ n ≤ 25
 *            a(26) = 44 * (a(1)+…+a(25)) – 989 a(n) = 44 * (a(n-1)+…+a(n-25))
 *            – 990 * a(n-26) for n ≥ 27 (All arithmetic is exact in seq_t,
 *            which may be a primitive or an arbitrary‑precision integer.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

/* cache for already computed terms */
static seq_t *cache_arr = NULL;
static size_t cache_cap = 0;    /* capacity of cache_arr */
static idx_t   cache_len = 0;   /* number of terms stored (0..len-1) */

/* enlarge cache to hold terms up to n (inclusive) and compute them */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;                           /* already computed */

    /* resize the array */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache_arr, new_cap * sizeof(seq_t));
        cache_arr = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms from cache_len to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;                           /* a(0) */
        } else if (i == 1) {
            val = (seq_t)46;                          /* a(1) */
        } else if (i <= 25) {
            /* a(i) = 45 * a(i-1) */
            val = (seq_t)45 * cache_arr[(size_t)(i - 1)];
        } else if (i == 26) {
            /* a(26) = 44 * sum_{k=1}^{25} a(k) – 989 */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 25; ++k)
                sum += cache_arr[(size_t)k];
            val = (seq_t)44 * sum - (seq_t)989;
        } else { /* i >= 27 */
            /* a(i) = 44 * sum_{k=1}^{25} a(i-k) – 990 * a(i-26) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 25; ++k)
                sum += cache_arr[(size_t)(i - k)];
            val = (seq_t)44 * sum - (seq_t)990 * cache_arr[(size_t)(i - 26)];
        }
        cache_arr[(size_t)i] = val;
    }

    cache_len = (idx_t) (n + 1);
}

/* public entry point */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache_arr[(size_t)n];
}
