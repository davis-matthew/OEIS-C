/*
 * A024307  a(n) = 2*t(n) + 3*t(n-1) + ... + (k+1)*t(n+1-k), where
 *          k=floor((n+1)/2) and t = A023531.
 * https://oeis.org/A024307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor((n+1)/2)} (k+1) * A023531(n+1-k)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A023531(m) = 1 iff m = k*(k+3)/2 for some integer k >= 1, else 0 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* integer square root for unsigned 64‑bit integers */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;          /* sqrt(n) <= n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 prod = (__int128)mid * mid;
        if (prod <= (__int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* memory helpers that work in C and C++ */
#ifdef __cplusplus
    #define FREE_SEQ(p) delete [] (p)
    #define ALLOC_SEQ(n) new seq_t[(n)]
    #define MEMCPY(dst, src, n) for (size_t i = 0; i < (n); ++i) (dst)[i] = (src)[i]
#else
    #define FREE_SEQ(p) free(p)
    #define ALLOC_SEQ(n) calloc((n), sizeof(seq_t))
    #define MEMCPY(dst, src, n) memcpy((dst), (src), (n) * sizeof(seq_t))
#endif

/* cached prefix sums of t(i) and i*t(i) */
static seq_t *S1 = NULL;   /* S1[i] = sum_{j=1..i} t(j)    */
static seq_t *S2 = NULL;   /* S2[i] = sum_{j=1..i} j*t(j)  */
static idx_t max_computed = 0;   /* largest index computed so far */
static size_t allocated = 0;     /* allocated array size */

/* grow the cache to cover up to n (inclusive) */
static void grow_to(idx_t n)
{
    size_t sz = (size_t)n + 1;
    if (sz <= allocated)
        return;

    seq_t *newS1 = ALLOC_SEQ(sz);
    seq_t *newS2 = ALLOC_SEQ(sz);

    if (S1) {
        MEMCPY(newS1, S1, allocated);
        MEMCPY(newS2, S2, allocated);
        FREE_SEQ(S1);
        FREE_SEQ(S2);
    }

    /* zero the new part */
    for (size_t i = allocated; i < sz; ++i) {
        newS1[i] = (seq_t)0;
        newS2[i] = (seq_t)0;
    }

    S1 = newS1;
    S2 = newS2;
    allocated = sz;
}

/* ensure the cache contains values up to n */
static void ensure_cache(idx_t n)
{
    if (n <= max_computed)
        return;

    grow_to(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* t(i) = 1 ⇔ 8*i+9 is an odd perfect square */
        idx_t val = 8 * i + 9;
        idx_t s   = isqrt_u64(val);
        bool flag = (s * s == val) && (s & 1u);
        seq_t ti   = flag ? (seq_t)1 : (seq_t)0;

        S1[i] = S1[i - 1] + ti;
        S2[i] = S2[i - 1] + (seq_t)i * ti;
    }

    max_computed = n;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* undefined but harmless */
    ensure_cache(n);

    idx_t start = (n + 2) / 2;                /* smallest argument of t in the sum */
    seq_t sum_t  = S1[n] - S1[start - 1];
    seq_t sum_it = S2[n] - S2[start - 1];

    return ((seq_t)(n + 2)) * sum_t - sum_it;
}
