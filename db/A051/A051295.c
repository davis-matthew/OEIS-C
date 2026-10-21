/*
 * A051295  a(0)=1; thereafter, a(m+1) = Sum_{k=0..m} k!*a(m-k).
 * https://oeis.org/A051295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1 a(n+1) = Sum_{k=0..n} k! * a(n-k) (for n >= 0)
 *            Equivalently, for m >= 0: a(m+1) = Sum_{k=0..m} k! * a(m-k) (m =
 *            n) a(0) = 1 This is an O(n^2) recurrence, but caching the
 *            factorials and the already-computed values allows computing a
 *            call in roughly Σ_{i=1..n} i operations, which stays far...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cached values of a[0..cached_n] and factorials fac[0..cached_n] */
static seq_t *a_cache   = NULL;
static seq_t *fac_cache = NULL;
static idx_t  cache_len = 0;  /* allocated size (>=1) */
static idx_t  cached_n  = 0;  /* highest index with defined a[n] */

/* grow the cached arrays (and zero‑initialise new slots) */
static void ensure_capacity(idx_t new_size)          /* new_size = needed index + 1 */
{
    if (new_size <= cache_len)
        return;      /* already enough */

    /* grow – a bit beyond the strictly needed size for future calls */
    idx_t new_len = new_size + 16;
    a_cache   = (seq_t *)realloc(a_cache,   new_len * sizeof(seq_t));
    fac_cache = (seq_t *)realloc(fac_cache, new_len * sizeof(seq_t));

    /* set new slots to zero – more than enough for subsequent init */
    for (idx_t i = cache_len; i < new_len; ++i) {
        a_cache[i]   = 0;
        fac_cache[i] = 0;
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0)              /* base term */
        return 1;

    if (n <= cached_n)               /* already computed earlier */
        return a_cache[n];

    /* ---------- make sure the arrays are large enough ----------
       we need space for indices 0..n  (size n+1)            */
    ensure_capacity(n + 1);

    /* ---------- initialise the very first element if not yet ---- */
    if (cached_n == 0) {
        a_cache[0]   = 1;
        fac_cache[0] = 1;          /* 0! = 1  */
        cached_n = 0;
    }

    /* ---------- extend factorials up to n ----------
       factorial[0] == 1 already set               */
    for (idx_t i = cached_n + 1; i <= n; ++i)
        fac_cache[i] = fac_cache[i - 1] * (seq_t)i;

    /* ---------- compute a[i] for i = cached_n+1 .. n ----------
       recurrence: a[i] = sum_{k=0}^{i-1} k! * a[i-1-k]       */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k)
            sum += fac_cache[k] * a_cache[i - 1 - k];
        a_cache[i] = sum;
    }

    cached_n = n;
    return a_cache[n];
}
