/*
 * A170542  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence for A170542:
 *   a(0) = 1
 *   For 1 ≤ n < 47 : a(n) = p(n) + 3 * Σ_{k=1}^{n} a(n-k)
 *     with p(0)=1 and p(n)=2 for 1 ≤ n ≤ 46.
 *   For n ≥ 47 : a(n) = 3 * Σ_{k=1}^{46} a(n-k) - 6 * a(n-47).
 *   (All arithmetic is in the integer type seq_t.)
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* persistent cache of computed terms */
static seq_t *cache = NULL;      /* array of a[i] for i ≤ max_cached */
static idx_t  max_cached = 0;    /* highest index that has been computed */
static size_t cache_cap = 0;     /* allocated capacity in elements */

/* Ensure the cache array can hold indices up to need-1 */
static void ensure_cache_size(idx_t need)
{
    size_t required = (size_t)need + 1;   /* need could be 0, allocate 1 element */
    if (!cache) {
        cache_cap = required > 8 ? required : 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failed */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        max_cached = 0;
    } else if (required > cache_cap) {
        cache_cap = required > cache_cap * 2 ? required : cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }
}

/* Compute terms up to index n (inclusive) */
static void compute_up_to(idx_t n)
{
    if (n <= max_cached)
        return;

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t sum = 0;
        if (i < 47) {
            /* sum over k=1..i of a[i-k] */
            for (idx_t k = 1; k <= i; ++k)
                sum += cache[i - k];
            seq_t p = (i == 0) ? (seq_t)1 : (seq_t)2;
            cache[i] = p + (seq_t)3 * sum;
        } else {
            /* sum over k=1..46 of a[i-k] */
            for (idx_t k = 1; k <= 46; ++k)
                sum += cache[i - k];
            cache[i] = (seq_t)3 * sum - (seq_t)6 * cache[i - 47];
        }
    }
    max_cached = n;
}

/* Public interface: return a(n) */
seq_t a(idx_t n)
{
    ensure_cache_size(n);
    compute_up_to(n);
    return cache[n];
}
