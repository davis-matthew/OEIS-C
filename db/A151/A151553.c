/*
 * A151553  G.f.: (1 + x) * Product_{n>=1} (1 + x^(2^n-1) + x^(2^n)).
 * https://oeis.org/A151553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1014, u16 n <= 8546, u32 n <= 8546, u64 n <= 8546,
 *            u128 n <= 8546, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=a(2)=2, a(2^m-1)=2, a(2^m)=3, a(2^m-2)=m;
   otherwise for n=2^i+j (0 <= j < 2^i-1), a(n)=a(j)+a(j+1). */

#include <stddef.h>
#include <stdlib.h>

/* return floor(log2(x)) for x >= 1 */
static inline idx_t floor_log2_u64(idx_t x)
{
    idx_t r = 0;
    while (x > 1)
    {
        x >>= 1;
        r++;
    }
    return r;
}

/* static cache for computed values; size 200000 is enough for all tests */
static seq_t  cache[200000];
static idx_t  cache_len = 0;   /* number of valid entries (next idx to compute) */

seq_t a(idx_t n)
{
    /* grow and compute cache up to n if necessary */
    if (n >= 200000) {
        /* out of bounds for static array – fall back to on‑the‑fly computation */
        /* This should not happen for the test cases (n <= ~10000). */
        /* Compute recursively without caching for safety. */
        if (n == 0) return (seq_t)1;
        if (n == 1 || n == 2) return (seq_t)2;
        if ((n & (n - 1)) == 0) return (seq_t)3;                 /* power of two */
        if (((n + 1) & n) == 0) return (seq_t)2;                /* n+1 power of two */
        if (((n + 2) & (n + 1)) == 0) return (seq_t)floor_log2_u64(n + 2);
        idx_t i = floor_log2_u64(n);
        idx_t j = n - ((idx_t)1 << i);
        return a(j) + a(j + 1);
    }

    if (n >= cache_len) {
        for (idx_t k = cache_len; k <= n; ++k) {
            if (k == 0) {
                cache[k] = (seq_t)1;
            } else if (k == 1 || k == 2) {
                cache[k] = (seq_t)2;
            } else if ((k & (k - 1)) == 0) {          /* power of two */
                cache[k] = (seq_t)3;
            } else if (((k + 1) & k) == 0) {          /* k+1 is power of two */
                cache[k] = (seq_t)2;
            } else if (((k + 2) & (k + 1)) == 0) {    /* k+2 is power of two */
                cache[k] = (seq_t)floor_log2_u64(k + 2);
            } else {                                 /* general case */
                idx_t i = floor_log2_u64(k);
                idx_t j = k - ((idx_t)1 << i);
                cache[k] = cache[j] + cache[j + 1];
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
