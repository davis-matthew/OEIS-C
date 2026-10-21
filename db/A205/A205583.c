/*
 * A205583  Number of (n+1) X 2 0..2 arrays with no 2 X 2 subblock having the
 *          same number of equal edges as its horizontal or vertical
 *          neighbors, and new values 0..2 introduced in row major order.
 * https://oeis.org/A205583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21 · a(n‑2) + 44 · a(n‑3) + 50 · a(n‑4) + 12 · a(n‑5) −
 *            144 · a(n‑6) for n > 7, with a(1)=14, a(2)=74, a(3)=413,
 *            a(4)=2268, a(5)=12574, a(6)=69338, a(7)=383440
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t computed   = 0;   /* number of terms currently cached (>= 0) */

/* first seven terms (1‑based) */
static const seq_t init_vals[7] = {
    (seq_t)14,
    (seq_t)74,
    (seq_t)413,
    (seq_t)2268,
    (seq_t)12574,
    (seq_t)69338,
    (seq_t)383440
};

/* Ensure the internal array has at least n positions.  */
static void ensure_alloc(size_t n)
{
    if (cache_cap >= n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n) new_cap *= 2;
    seq_t *new_cache = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term of OEIS sequence A205583 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined, but harmless */
    if ((size_t)n <= 7) return init_vals[n-1];   /* initial terms */

    /* initialise cache with the first seven terms if not already done */
    if (computed < 7) {
        ensure_alloc(7);
        for (size_t i = 0; i < 7; ++i) cache[i] = init_vals[i];
        computed = 7;
    }

    /* extend cache to the requested index */
    size_t target = (size_t)n;
    if (target > computed) {
        ensure_alloc(target);
        for (size_t i = computed; i < target; ++i) {
            /* i corresponds to a(i+1) */
            seq_t val =
                (seq_t)21  * cache[i-2]      /* a(i‑1) */
              + (seq_t)44  * cache[i-3]      /* a(i‑2) */
              + (seq_t)50  * cache[i-4]      /* a(i‑3) */
              + (seq_t)12  * cache[i-5]      /* a(i‑4) */
              - (seq_t)144 * cache[i-6];     /* a(i‑5) */
            cache[i] = val;
        }
        computed = target;
    }

    return cache[n-1];
}
