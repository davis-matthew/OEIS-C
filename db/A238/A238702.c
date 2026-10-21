/*
 * A238702  Sum of the smallest parts of the partitions of 4n into 4 parts.
 * https://oeis.org/A238702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 5*a(n-4) + 6*a(n-5) * -
 *            4*a(n-6) + a(n-7) for n ≥ 8, * with a(1)=1, a(2)=6, a(3)=21,
 *            a(4)=55, a(5)=119, * a(6)=227, a(7)=396
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    /* Local cast to size_t for index arithmetic */
    size_t N = (size_t)n;

    /* Static cache storing computed values; cache[i] is a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of stored elements (>=7 after init) */
    static size_t cache_cap   = 0;   /* allocated capacity */

    /* Grow capacity if needed */
    if (N > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < N) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first seven terms (only once) */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;     /* a(1) */
        cache[1] = (seq_t)6;     /* a(2) */
        cache[2] = (seq_t)21;    /* a(3) */
        cache[3] = (seq_t)55;    /* a(4) */
        cache[4] = (seq_t)119;   /* a(5) */
        cache[5] = (seq_t)227;   /* a(6) */
        cache[6] = (seq_t)396;   /* a(7) */
        cache_size = 7;
    }

    /* If the requested value is already cached, return it */
    if (N <= cache_size)
        return cache[N-1];

    /* Compute missing terms up to N using the recurrence */
    /* Recurrence is valid for n ≥ 8, so start from max(cache_size+1, 8) */
    size_t start = cache_size + 1;
    if (start < 8) start = 8;
    for (size_t i = start; i <= N; ++i) {
        /* i corresponds to a(i); indices are offset by -1 in cache */
        seq_t val = 4 * cache[i-2];    /* a(i-1) */
        val -= 6 * cache[i-3];         /* a(i-2) */
        val += 5 * cache[i-4];         /* a(i-3) */
        val -= 5 * cache[i-5];         /* a(i-4) */
        val += 6 * cache[i-6];         /* a(i-5) */
        val -= 4 * cache[i-7];         /* a(i-6) */
        val +=  cache[i-8];            /* a(i-7) */
        cache[i-1] = val;
    }
    /* Update stored size to the largest computed value */
    cache_size = N;

    return cache[N-1];
}
