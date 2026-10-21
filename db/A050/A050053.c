/*
 * A050053  a(n) = a(n-1) + a(m) for n >= 4, where m = 2^(p+1) + 2 - n and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1, a(2) = 2, and a(3) = 4.
 * https://oeis.org/A050053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=4; for n ≥ 4, a(n) = a(n-1) + a(m) where m
 *            = 2^(p+1) + 2 – n and p is the unique integer with 2^p < n‑1 ≤
 *            2^(p+1)
 * Range:     u8 n <= 18, u16 n <= 71, u32 n <= 515, u64 n <= 4297,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cached_vals = NULL; /* values of a(n) */
static bool   *cached_ok  = NULL; /* whether a(n) is computed */
static size_t  cache_cap  = 0;

/* Grow the cache to accommodate values up to idx (1‑based). */
static void ensure_cache(idx_t idx)
{
    if ((size_t)idx <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)idx) new_cap *= 2;
    cached_vals = (seq_t *)realloc(cached_vals, new_cap * sizeof(seq_t));
    cached_ok  = (bool  *)realloc(cached_ok,  new_cap * sizeof(bool ));
    for (size_t i = cache_cap; i < new_cap; ++i) cached_ok[i] = false;
    cache_cap = new_cap;
}

/* Recursive implementation with memoization. */
static seq_t compute_a(idx_t n)
{
    /* Base cases. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    ensure_cache(n);
    if (cached_ok[n-1]) return cached_vals[n-1];

    /* a(n-1) needed first. */
    seq_t prev = compute_a(n-1);

    /* Determine p: 2^p < n-1 <= 2^(p+1). */
    idx_t n1 = n-1;
    unsigned long long tmp = (unsigned long long)(n1-1); /* n1-1 ≥ 2 for n≥4 */
    unsigned int p = 63 - __builtin_clzll(tmp);

    /* Compute m = 2^(p+1) + 2 - n. */
    idx_t two_pow = ((idx_t)1) << (p+1);
    idx_t m = two_pow + 2 - n;

    /* Recursive call for a(m). */
    seq_t val = prev + compute_a(m);

    cached_vals[n-1] = val;
    cached_ok[n-1]   = true;
    return val;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    return compute_a(n);
}
