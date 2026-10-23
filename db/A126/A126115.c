/*
 * A126115  E.g.f.: sqrt(1+2*x)/(1-2*x).
 * https://oeis.org/A126115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A126115
   a(n) = a(n-2)*2*(n-1)*(1+2*(n-2)) + a(n-1)*3  (n >= 2)
   a(0) = 1,  a(1) = 3
   Recurrence:  a(n+2) = (2*(n+1))*(1+2*n)*a(n) + 3*a(n+1)  ,  a(0)=1, a(1)=3
*/

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
/* For arbitrary‑precision seq_t (C++): use new/delete */
# define seq_alloc(cap)      new seq_t[(cap)]
# define seq_free(ptr)       delete[] (ptr)
#else
/* For built‑in integral seq_t (C): use malloc/free */
# define seq_alloc(cap)      (seq_t*)malloc((cap) * sizeof(seq_t))
# define seq_free(ptr)       free(ptr)
#endif

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache   = NULL;   /* array of computed a(k) */
    static idx_t  cap     = 0;      /* current capacity of cache   */
    static idx_t  last    = (idx_t)-1; /* highest index computed so far */

    /* initialise cache on first call */
    if (cap == 0) {
        cap = 2;                    /* store a(0) and a(1)          */
        cache = seq_alloc(cap);
        if (!cache) abort();          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        last = 1;
    }

    /* grow cache if necessary */
    if (n >= cap) {
        idx_t newcap = n + 1;
        if (newcap < cap * 2) newcap = cap * 2;   /* exponential growth */
        seq_t *newc = seq_alloc(newcap);
        for (idx_t i = 0; i <= last; ++i)
            newc[i] = cache[i];
        seq_free(cache);
        cache = newc;
        cap = newcap;
    }

    /* already computed? */
    if (n <= last)
        return cache[n];

    /* compute remaining values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* i >= 2  because a(i-2) and a(i-1) exist */
        idx_t k = i - 2;                     /* k = i-2 */
        seq_t term2 = (seq_t)3 * cache[i - 1];
        seq_t mult   = (seq_t)2 * ((seq_t)k + (seq_t)1) * ((seq_t)1 + (seq_t)2 * (seq_t)k);
        seq_t term1  = mult * cache[i - 2];
        cache[i] = term1 + term2;
    }
    last = n;
    return cache[n];
}
