/*
 * A077840  Expansion of (1-x)/(1-2*x-3*x^2-3*x^3).
 * https://oeis.org/A077840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=5 a(n)=2*a(n-1)+3*a(n-2)+3*a(n-3) for n≥3
 *            (OEIS A077840 – Expansion of (1‑x)/(1‑2x‑3x²‑3x³).)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define CACHE_LIMIT 10000   /* sufficient for all test values */

/* Static cache for up to CACHE_LIMIT.  The cache may be partially filled
   if the user calls a(idx_t) with n > CACHE_LIMIT – in that case we
   compute on the fly without extending the array. */
static seq_t cache[CACHE_LIMIT + 1];
static idx_t cache_last = (idx_t)-1;  /* highest n for which cache[n] is valid */

/* Ensure cache[0..n] (≤CACHE_LIMIT) is computed.  If n exceeds the
   cache size, we simply compute the requested value without storing
   it beyond the array bounds. */
static void ensure_cache(idx_t n) {
    /* Handle the trivial cases directly */
    if (n == 0) { cache[0] = (seq_t)1; cache_last = 0; return; }
    if (n == 1) { cache[0] = (seq_t)1; cache[1] = (seq_t)1; cache_last = 1; return; }
    if (n == 2) { cache[0] = (seq_t)1; cache[1] = (seq_t)1; cache[2] = (seq_t)5; cache_last = 2; return; }

    /* Determine if we can use the static cache. */
    if (n <= CACHE_LIMIT) {
        /* If cache not yet initialized, set base terms. */
        if (cache_last < 2) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)5;
            cache_last = 2;
        }
        /* Compute remaining terms up to n, filling the cache. */
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t v1 = cache[i-1];
            seq_t v2 = cache[i-2];
            seq_t v3 = cache[i-3];
            cache[i] = (seq_t)2 * v1 + (seq_t)3 * v2 + (seq_t)3 * v3;
        }
        cache_last = n;
        return;
    }

    /* n exceeds the static cache size – compute iteratively without storing
       beyond CACHE_LIMIT.  We keep only the last three values needed for
       the recurrence. */
    /* We need the values at indices 2, 1, 0 as starting points, but we
       let the cache hold them if it exists. */
    seq_t a0, a1, a2;
    if (cache_last >= 2) {
        a0 = cache[2];
        a1 = cache[1];
        a2 = cache[0];
    } else {
        a0 = (seq_t)5;
        a1 = (seq_t)1;
        a2 = (seq_t)1;
    }

    /* Compute forward */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)2 * a0 + (seq_t)3 * a1 + (seq_t)3 * a2;
        a2 = a1;
        a1 = a0;
        a0 = next;
    }
    /* The result is now in a0.  Since we don't cache beyond the limit,
       we simply return it. */
    cache[0] = a0; /* store as a temporary for consistency with cache usage */
    cache_last = (idx_t)-1; /* indicate cache is no longer valid for larger n */
}

/* Public sequence function.  It calls ensure_cache to guarantee that
   cache[n] (if n ≤ CACHE_LIMIT) contains the correct value. */
seq_t a(idx_t n) {
    if (n <= CACHE_LIMIT) {
        ensure_cache(n);
        return cache[n];
    } else {
        /* For n > CACHE_LIMIT, compute iteratively without storing. */
        seq_t a0, a1, a2;
        if (cache_last >= 2) {
            a0 = cache[2];
            a1 = cache[1];
            a2 = cache[0];
        } else {
            a0 = (seq_t)5;
            a1 = (seq_t)1;
            a2 = (seq_t)1;
        }
        for (idx_t i = 3; i <= n; ++i) {
            seq_t next = (seq_t)2 * a0 + (seq_t)3 * a1 + (seq_t)3 * a2;
            a2 = a1;
            a1 = a0;
            a0 = next;
        }
        return a0;
    }
}
