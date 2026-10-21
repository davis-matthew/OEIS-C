/*
 * A074323  Coefficient of the highest power of q in the expansion of nu(0)=1,
 *          nu(1)=b and for n>=2, nu(n)=b*nu(n-1)+lambda*(n-1)_q*nu(n-2) with
 *          (b,lambda)=(1,2), where (n)_q=(1+q+...+q^(n-1)) and q is a root of
 *          unity.
 * https://oeis.org/A074323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A074323 – coefficient of the highest power of q in ν(n) for b=1, λ=2.
   Formula: a(0)=1, a(1)=1;
           if n is odd: a(n)=2^((n-1)/2);
           if n is even: a(n)=3*a(n-1).

   The sequence is defined recursively, but the closed form above allows
   O(n) generation with memoisation. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache allows random‑order look‑ups while keeping the
       pre‑allocated data in a contiguous array. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values actually stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Grow the cache if needed */
    if (cache_len <= n) {
        idx_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap <= n) new_cap <<= 1;

        if (cache == NULL)
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;

        /* Compute all missing values in ascending order */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i & 1) {          /* odd index */
                /* 2^((i-1)/2) – shift by half the gap from 1 */
                cache[i] = ((seq_t)1) << ((i - 1) >> 1);
            } else {                      /* even index */
                cache[i] = cache[i - 1] * (seq_t)3;
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
