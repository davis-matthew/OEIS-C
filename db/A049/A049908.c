/*
 * A049908  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 3.
 * https://oeis.org/A049908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A049908 – recurrence definition
 * a(1) = 1, a(2) = 2, a(3) = 3
 * For n ≥ 4: 
 *   a(n) = a(1)+a(2)+…+a(n‑1) – a(m)
 *   where m = 2*n – 3 – 2^(p+1)
 *   and p is the unique integer with 2^p < n‑1 ≤ 2^(p+1).
 *   (p = floor_log2(n‑2))
 *
 * This implementation stores previously computed terms and a running
 * prefix sum so that each new term is calculated in O(1).  It works
 * for any unsigned integer type seq_t and any unsigned 64‑bit index
 * type idx_t, compiling under C11 or C++17.
 */

#include <stddef.h>
#include <stdlib.h>

/* static state: cached values, capacity, last computed index, and
 * running prefix sum of a(1) … a(last_n).
 */
static seq_t *a_cache   = NULL;   /* 1‑based storage for a(n) */
static size_t cache_cap = 0;      /* current allocation size of a_cache */
static idx_t last_n     = 0;      /* largest n already computed */
static seq_t sum_last   = (seq_t)0; /* sum_{k=1}^{last_n} a(k) */

/* floor_log2 for non‑zero idx_t values – returns largest p with 2^p ≤ v */
static inline unsigned int floor_log2_idx(idx_t v)
{
    unsigned int p = 0;
    while (v >>= 1) ++p;
    return p;
}

/* Ensure a_cache has room for indices up to n (0‑based array of size n+1). */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > (size_t)n + 1) return;   /* already sufficient */
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < (size_t)n + 1) new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* public function: compute a(n) with caching and memoisation. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* safety for out‑of‑range indices */

    ensure_capacity(n);

    /* Return cached value if already computed. */
    if (n <= last_n) return a_cache[n];

    /* Compute terms from last_n+1 up to n inclusive. */
    for (idx_t i = last_n + 1; i <= n; ++i)
    {
        seq_t val;
        if (i == 1)          val = (seq_t)1;
        else if (i == 2)     val = (seq_t)2;
        else if (i == 3)     val = (seq_t)3;
        else
        {
            unsigned int p   = floor_log2_idx(i - 2);
            idx_t two_pow    = ((idx_t)1) << (p + 1);          /* 2^(p+1)   */
            idx_t m          = (idx_t)(2 * i - 3) - two_pow;    /* m = 2n‑3‑2^(p+1) */
            val = sum_last - a_cache[m];
        }

        a_cache[i] = val;
        sum_last += val;   /* update running prefix sum */
    }

    last_n = n;
    return a_cache[n];
}
