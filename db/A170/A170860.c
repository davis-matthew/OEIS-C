/*
 * A170860  a(0) = 1, a(1) = 0; a(2^i + j) = a(j) + 3a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 1, a(1) = 0; a(2^i + j) = a(j) + 3·a(j+1) for 0 ≤ j <
 *            2^i. The function below computes a(n) for any n ≥ 0 using an
 *            iterative dynamic‑programming cache
 * Range:     u8 n <= 58, u16 n <= 1015, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/floor_log2_uint64.h"

/* cache of already computed values */
static seq_t *cache = NULL;   /* array of seq_t */
static size_t cap   = 0;      /* current capacity of cache  */
static idx_t  max_computed = 0;  /* largest index that has been filled */

/* Allocate or grow the cache to hold at least 'need' elements. */
static void grow_to(size_t need)
{
    if (need <= cap)
        return;

    size_t newcap = (cap > 0) ? cap : 1;
    while (newcap < need)
        newcap <<= 1;

    seq_t *tmp = seq_alloc_array(newcap);
    if (cache) {
        for (size_t i = 0; i < cap; ++i)
            tmp[i] = cache[i];
        /* seq_free_array does not free memory, it's optional;
           it is safe to skip freeing in this context. */
    }
    cache = tmp;
    cap   = newcap;
}

/* 
   Compute a(n) for any n, using the cached values up to max_computed
   and filling gaps iteratively.  No recursion is used, which removes
   the previous bug caused by incorrect memoisation order.
*/
seq_t a(idx_t n)
{
    /* Lazy initialisation of cache and base values. */
    if (cache == NULL) {
        grow_to(1);            /* room for a(0) */
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        max_computed = 0;
    }

    if (n >= 1 && max_computed < 1) {
        grow_to(2);            /* room for a(1) */
        cache[1] = (seq_t)0;   /* a(1) = 0 */
        max_computed = 1;
    }

    if (n <= max_computed)        /* already computed */
        return cache[n];

    /* Make sure the cache can hold index n. */
    grow_to((size_t)n + 1);

    /* Compute missing values up to n inclusively. */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k < 2) continue;   /* a(0) and a(1) are already set */

        idx_t i = floor_log2_uint64(k);       /* i such that 2^i ≤ k < 2^(i+1) */
        idx_t m = 1ULL << i;                  /* 2^i */
        idx_t j = k - m;                      /* 0 ≤ j < 2^i */

        cache[k] = cache[j] + (seq_t)3 * cache[j + 1];
    }

    max_computed = n;
    return cache[n];
}
