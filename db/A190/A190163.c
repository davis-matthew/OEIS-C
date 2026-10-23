/*
 * A190163  Number of subwords of type dh^ju (j>=1), where u=(1,1), h=(1,0),
 *          and d=(1,-1), in all peakless Motzkin paths of length n (can be
 *          easily expressed using RNA secondary structure terminology).
 * https://oeis.org/A190163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            (c1*a(n-1)+c2*a(n-2)+c3*a(n-3)+c4*a(n-4)+c5*a(n-5)+c6*a(n-6)) /
 *            (4*(n+1)*(n-7)) for n>=8 where c1 = 13*n^2-85*n+28 c2 =
 *            -7*n^2+52*n-41 c3 = 5*n^2-41*n+67 c4 = -13*n^2+103*n-197 c5 =
 *            (7*n-29)*(n-5) c6 = -(n-5)*(n-6) Initial values:
 *            a(0)=a(1)=...=a(6)=0, a(7)=1
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 20, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;   /* number of entries computed (next free) */

    /* Initialise cache with the known initial values. */
    if (cap == 0) {
        cap = 8;                      /* 0..7 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        memset(cache, 0, cap * sizeof(seq_t));  /* a(0)..a(6)=0   */
        cache[7] = (seq_t)1;          /* a(7)=1          */
        len = 8;                      /* next index to compute */
    } else if (cap <= (size_t)n) {
        /* Grow to at least n+1 elements. */
        size_t newcap = cap * 2;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *newcache = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!newcache) return cache[n < len ? n : 0];
        memcpy(newcache, cache, cap * sizeof(seq_t));
        memset(newcache + cap, 0, (newcap - cap) * sizeof(seq_t));
        free(cache);
        cache = newcache;
        cap   = newcap;
    }

    if (n < len) return cache[n];

    for (size_t idx = len; idx <= (size_t)n; ++idx) {
        if (idx < 8) {                  /* safety, should not happen */
            cache[idx] = (seq_t)0;
            continue;
        }

        /* Compute integer coefficients for this n. */
        int64_t m   = (int64_t)idx;
        int64_t c1  = 13LL*m*m - 85LL*m + 28;
        int64_t c2  = -7LL*m*m + 52LL*m - 41;
        int64_t c3  = 5LL*m*m - 41LL*m + 67;
        int64_t c4  = -13LL*m*m + 103LL*m - 197;
        int64_t c5  = (7LL*m - 29LL) * (m - 5LL);
        int64_t c6  = -(m - 5LL) * (m - 6LL);

        /* Sum = c1*a(idx-1)+c2*a(idx-2)+...+c6*a(idx-6) */
        seq_t pos = (seq_t)0, neg = (seq_t)0;

        /* a(idx-1) with c1 (positive or negative) */
        if (c1 >= 0) pos += cache[idx - 1] * (seq_t)c1;
        else         neg += cache[idx - 1] * (seq_t)(-c1);

        /* a(idx-2) with c2 */
        if (c2 >= 0) pos += cache[idx - 2] * (seq_t)c2;
        else         neg += cache[idx - 2] * (seq_t)(-c2);

        /* a(idx-3) with c3 */
        if (c3 >= 0) pos += cache[idx - 3] * (seq_t)c3;
        else         neg += cache[idx - 3] * (seq_t)(-c3);

        /* a(idx-4) with c4 */
        if (c4 >= 0) pos += cache[idx - 4] * (seq_t)c4;
        else         neg += cache[idx - 4] * (seq_t)(-c4);

        /* a(idx-5) with c5 */
        if (c5 >= 0) pos += cache[idx - 5] * (seq_t)c5;
        else         neg += cache[idx - 5] * (seq_t)(-c5);

        /* a(idx-6) with c6 */
        if (c6 >= 0) pos += cache[idx - 6] * (seq_t)c6;
        else         neg += cache[idx - 6] * (seq_t)(-c6);

        seq_t numer = pos - neg; /* guaranteed non‑negative for n≥8 */

        /* Denominator: 4*(n+1)*(n-7)  (n=idx>=8 so positive) */
        seq_t denom = 4 * (seq_t)(idx + 1) * (seq_t)(idx - 7);

        cache[idx] = numer / denom;
    }

    len = (size_t)n + 1;
    return cache[n];
}
