/*
 * A080254  For n>3, a(n) is the number of elements in the Coxeter complex of
 *          type D_n (although the sequence starts at n=0. See comments below
 *          for precise explanation).
 * https://oeis.org/A080254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080254:   a(0)=a(1)=1; a(n)=1+∑_{r=1..n} 2^r·binomial(n,r)·a(n−r)  for n>1 */
#include <stdlib.h>
/* seq_t and idx_t are supplied by the build environment.  */

seq_t a(idx_t n)
{
    /* static memoisation cache: a[0]..a[last]  */
    static seq_t *cache = NULL;      /* array holding computed values   */
    static size_t cap   = 0;         /* allocated size of cache        */
    static idx_t last   = (idx_t)-1; /* largest n already computed     */

    /* values for n = 0 or 1 are defined directly  */
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }

    /* extend cache if necessary */
    if (cap < (size_t)n + 1) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp && newcap > 0) {
            abort(); /* memory allocation failure */
        }
        cache = tmp;
        cap   = newcap;
    }

    /* initialise base cases once */
    if (last == (idx_t)-1) {
        cache[0] = (seq_t)1;
        if (cap > 1) {
            cache[1] = (seq_t)1;
        }
        last = 1;
    }

    /* already computed  */
    if (n <= last) {
        return cache[n];
    }

    /* compute terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence holds for i >= 2  */
        seq_t sum   = 0;
        seq_t binom = 1;     /* binomial(i,0) */
        seq_t pow2  = 2;     /* 2^1 */

        for (idx_t r = 1; r <= i; ++r) {
            /* binom(i,r) = binom(i,r-1) * (i-r+1) / r   */
            binom = binom * (i - r + 1) / r;

            /* term = a(i-r) * 2^r * binom(i,r) */
            seq_t term = cache[i - r];
            term = term * pow2;
            term = term * binom;

            sum += term;

            pow2 <<= 1; /* next power of two */
        }

        cache[i] = 1 + sum;
    }

    last = n;
    return cache[n];
}
