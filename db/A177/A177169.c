/*
 * A177169  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=7, k=0 and l=-2.
 * https://oeis.org/A177169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177169
 *
 * Recurrence (for n ≥ 1):
 *     a(n+1) = Σ_{p=0}^{n} a(p)*a(n-p) – 2
 *
 * Holonomic recurrence (for n ≥ 5):
 *     (n+1)*a(n) = 2*(3n-1)*a(n-1) + 5*(3n-7)*a(n-2)
 *                 – 2*(26n-81)*a(n-3) + 32*(n-4)*a(n-4)
 *
 * Initial values:
 *     a(0) = 1,  a(1) = 7,  a(2) = 12,  a(3) = 71,  a(4) = 308
 *
 * This implementation memoises the sequence in a static cache to allow
 * calls in arbitrary order.  All arithmetic is performed with the
 * provided integer type `seq_t`; no assumptions are made about its
 * width (it may be 8, 16, 32, 64, 128 bits or arbitrary‑precision)
 * and the function works for all supported builds.
 */

#include <stdlib.h>  /* for realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* memoised sequence values      */
    static idx_t cap   = 0;      /* size of cache (>= number stored) */

    /* If the requested index is already computed, return it. */
    if (n < cap)
        return cache[n];

    /* Grow the cache to accommodate indices up to n (inclusive). */
    size_t newcap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) /* allocation failure – return 0 as a sentinel. */
        return (seq_t)0;
    cache = tmp;

    /* Compute new terms from cap up to n. */
    for (idx_t i = cap; i <= n; ++i) {
        if (i == 0)        cache[i] = (seq_t)1;
        else if (i == 1)   cache[i] = (seq_t)7;
        else if (i == 2)   cache[i] = (seq_t)12;
        else if (i == 3)   cache[i] = (seq_t)71;
        else if (i == 4)   cache[i] = (seq_t)308;
        else {
            /* Linear coefficients in the holonomic recurrence. */
            long long c1 = 6 * (long long)i - 2;   /* 2*(3i-1) */
            long long c2 = 15 * (long long)i - 35; /* 5*(3i-7) */
            long long c3 = 52 * (long long)i - 162;/* 2*(26i-81) */
            long long c4 = 32 * (long long)i - 128;/* 32*(i-4)  */

            seq_t t1 = cache[i-1] * (seq_t)c1;
            seq_t t2 = cache[i-2] * (seq_t)c2;
            seq_t t3 = cache[i-3] * (seq_t)c3;
            seq_t t4 = cache[i-4] * (seq_t)c4;

            /* numer = t1 + t2 + t4 - t3  (t3 is subtracted) */
            seq_t numer = t1 + t2 + t4 - t3;
            cache[i] = numer / (seq_t)(i + 1);
        }
    }

    cap = newcap;  /* update capacity for future calls */
    return cache[n];
}
