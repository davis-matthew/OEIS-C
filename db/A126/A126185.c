/*
 * A126185  Number of nonroot nodes of outdegree 2 in all hex trees with n
 *          edges.
 * https://oeis.org/A126185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=a(2)=0, a(3)=3, for n≥4: a(n)=((7*n+1)*(n-2)*a(n-1)
 *            -(11*n-15)*(n-2)*a(n-2) +5*(n-2)*(n-3)*a(n-3))/((n+2)*(n-3))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=3 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* cache up to 10000; grows lazily if needed */
    static seq_t cache[10000 + 1];
    static size_t cache_len = 0;      /* highest index computed so far */

    /* initialise base cases once */
    if (!cache_len) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)3;
        cache_len = 3;
    }

    /* extend the cache to n if necessary */
    if (n > cache_len) {
        /* simple linear extension loop */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t ns = (seq_t)i;                     /* cast index to seq_t */
            seq_t t1 = ((seq_t)7 * ns + (seq_t)1) * (ns - (seq_t)2) * cache[i - 1];
            seq_t t2 = ((seq_t)11 * ns - (seq_t)15) * (ns - (seq_t)2) * cache[i - 2];
            seq_t t3 = (seq_t)5 * (ns - (seq_t)2) * (ns - (seq_t)3) * cache[i - 3];
            seq_t numer = t1 - t2 + t3;
            seq_t denom = (ns + (seq_t)2) * (ns - (seq_t)3);
            cache[i] = numer / denom;      /* exact division */
        }
        cache_len = n;
    }

    return cache[n];
}
