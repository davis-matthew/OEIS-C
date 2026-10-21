/*
 * A072979  a(n) = Sum_{k=1..n-1} gcd(k,n)*a(k), a(1) = 1.
 * https://oeis.org/A072979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = Sum_{k=1}^{n-1} gcd(k,n)*a(k) , a(1)=1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 55, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the values in a static array so that
   the function can be called in any order.  The array uses 1‑based
   indexing: cache[1] holds a(1).  The function works for all
   integer types that can represent the sequence values exactly;
   intermediate arithmetic is performed in the same type, so for
   built‑in 64‑bit types it is safe as long as the result itself
   fits in 64 bits (the build is then tested only for such n). */

#include <stddef.h>
#include <stdlib.h>

/* Small Euclidean GCD for idx_t values (≤ 64 bit).  */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --------------------------------------------------------------------- */
/*  Memoisation cache                                               */
/* --------------------------------------------------------------------- */

static seq_t *cache      = NULL;   /* 1‑based array of a(n) values   */
static size_t cache_size = 0;      /* allocated size (≥ last+1)      */
static idx_t   cache_last = 0;     /* largest index that has been   */
                                   /* computed (cache[cache_last] is valid) */

/* Ensure that the cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache_last >= n)
        return;                     /* already computed */

    /* Grow the array if needed.  We allocate at least n+1 elements
       so that index n is valid.  The growth strategy is simple:
       extend exactly to n+1; this keeps the code short and the
       allocation behaviour deterministic. */
    size_t needed = (size_t)(n + 1);          /* 1‑based index */
    if (cache_size < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();                    /* out of memory */
        cache = tmp;
        cache_size = needed;
    }

    /* Compute missing entries.  For i == 1 we set the known seed value. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            idx_t g = gcd_idx(k, i);        /* gcd as idx_t */
            sum += cache[k] * (seq_t)g;     /* natural arithmetic in seq_t */
        }
        cache[i] = sum;
    }

    cache_last = n;
}

/* --------------------------------------------------------------------- */
/*  Public function                                                   */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but return 0 safely */

    ensure_cache(n);
    return cache[n];
}
