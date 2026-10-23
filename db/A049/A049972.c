/*
 * A049972  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1 and a(2) = a(3) = 3.
 * https://oeis.org/A049972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A049972 * a(1) = 1, a(2) = a(3) = 3 * For n ≥ 4 * let p be the
 *            unique integer with 2^p < n‑1 ≤ 2^(p+1) * let m = 2·n – 3 –
 *            2^(p+1) * then a(n) = a(1)+a(2)+…+a(n‑1) + a(m) * * The
 *            implementation memoises all values up to the largest n *
 *            requested. It uses a prefix‑sum array so that a(n) can be * c...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

/* --- Utility: floor(log2) for 64-bit unsigned integers --- */
static inline idx_t floor_log2_idx(idx_t v)
{
    idx_t r = 0;
    /* v must be ≥ 2: shift until zero */
    while (v >>= 1) ++r;
    return r;
}

/* --- Memoisation buffers --- */
static seq_t *a_cache = NULL;        /* a[1..] */
static seq_t *pref_cache = NULL;     /* prefix sums of a */
static idx_t  cache_size = 0;        /* number of indices currently stored */

/* Ensure that a_cache and pref_cache contain values up to index n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;                /* already computed */

    /* Allocate/resize arrays to hold indices 0..n (0 unused) */
    size_t new_sz = (size_t)(n + 1);
    a_cache    = (seq_t *)realloc(a_cache,    new_sz * sizeof(seq_t));
    pref_cache = (seq_t *)realloc(pref_cache, new_sz * sizeof(seq_t));

    /* Compute missing entries */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            a_cache[i]    = (seq_t)1;
            pref_cache[i] = a_cache[i];
        } else if (i == 2 || i == 3) {
            a_cache[i]    = (seq_t)3;
            pref_cache[i] = pref_cache[i - 1] + a_cache[i];
        } else {
            /* p = floor(log2(n-2)) because n-1 > 2^p */
            idx_t p   = floor_log2_idx(i - 2);
            idx_t pow = (idx_t)1 << (p + 1);            /* 2^(p+1) */
            idx_t m   = (idx_t)2 * i - 3 - pow;        /* 2n-3-2^(p+1) */

            /* Recursive definition using the prefix sum and a[m] */
            a_cache[i]    = pref_cache[i - 1] + a_cache[m];
            pref_cache[i] = pref_cache[i - 1] + a_cache[i];
        }
    }
    cache_size = n;
}

/* External function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, safe guard */
    ensure_cache(n);
    return a_cache[n];
}
