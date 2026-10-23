/*
 * A026085  a(n) = number of (s(0), s(1), ..., s(n)) such that every s(i) is
 *          an integer, s(0) = 0, |s(i) - s(i-1)| = 1 for i = 1,2,3; |s(i) -
 *          s(i-1)| <= 1 for i >= 4, s(n) = 2. Also a(n) = T(n,n-2), where T
 *          is the array defined in A026082.
 * https://oeis.org/A026085
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies (n+2)a(n)= (5n+1)a(n-1) – 4(n-4)a(n-2) * –
 *            8(n-1)a(n-3) + (5n-34)a(n-4) + (3n-21)a(n-5) for n≥10 * with
 *            a(4)=4, a(5)=8, a(6)=27, a(7)=76, a(8)=226, a(9)=660
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Empty or negative indices are undefined; return 0 by convention. */
    if (n < 4) return (seq_t)0;

    /* Static cache holds values from a(4) upwards. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of entries currently stored */
    static idx_t max_cached = 0;     /* greatest n for which a(n) is cached */

    /* Initialise cache with the first six known terms. */
    if (cache == NULL) {
        cache_len = 6;               /* indices 4..9 inclusive */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)4;         /* a(4) */
        cache[1] = (seq_t)8;         /* a(5) */
        cache[2] = (seq_t)27;        /* a(6) */
        cache[3] = (seq_t)76;        /* a(7) */
        cache[4] = (seq_t)226;       /* a(8) */
        cache[5] = (seq_t)660;       /* a(9) */
        max_cached = 9;
    }

    /* If already computed, just return the cached value. */
    if (n <= max_cached)
        return cache[n - 4];

    /* Grow the cache to accommodate up to n. */
    idx_t required_len = n - 4 + 1;
    if (required_len > cache_len) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * required_len);
        cache_len = required_len;
    }

    /* Compute values from max_cached+1 up to n using the recurrence. */
    for (idx_t cur = max_cached + 1; cur <= n; ++cur) {
        /* Indices into the cache array. */
        idx_t i   = cur - 4;        /* cache[i] = a(cur)               */
        idx_t i1  = i - 1;          /* a(cur-1) */
        idx_t i2  = i - 2;          /* a(cur-2) */
        idx_t i3  = i - 3;          /* a(cur-3) */
        idx_t i4  = i - 4;          /* a(cur-4) */
        idx_t i5  = i - 5;          /* a(cur-5) */

        /* Previous terms. */
        seq_t a1 = cache[i1];
        seq_t a2 = cache[i2];
        seq_t a3 = cache[i3];
        seq_t a4 = cache[i4];
        seq_t a5 = cache[i5];

        /* Coefficients as seq_t (they may be large, but cast from idx_t). */
        seq_t cA = (seq_t)(5 * cur + 1);          /* (5n+1)   */
        seq_t cB = (seq_t)(4 * (cur - 4));        /* 4(n-4)   */
        seq_t cC = (seq_t)(8 * (cur - 1));        /* 8(n-1)   */
        seq_t cD = (seq_t)(5 * cur - 34);        /* 5n-34    */
        seq_t cE = (seq_t)(3 * cur - 21);        /* 3n-21    */

        /* Numerator of the recurrence. */
        seq_t num = cA * a1
                  - cB * a2
                  - cC * a3
                  + cD * a4
                  + cE * a5;

        /* Denominator (n+2). */
        seq_t den = (seq_t)(cur + 2);

        cache[i] = num / den;
    }

    max_cached = n;
    return cache[n - 4];
}
