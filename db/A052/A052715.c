/*
 * A052715  Expansion of e.g.f. (1-2*x-sqrt(1-4*x))/2 - x*(1-2*x-sqrt(1-4*x))
 *          - x^2.
 * https://oeis.org/A052715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A052715
    Recurrence: a(0)=a(1)=a(2)=a(3)=0, a(4)=24.
                 For n ≥ 3:
                     a(n+2) = -(4*(-n-3+2*n^2)*a(n) + 2*(-1-3*n)*a(n+1))
                 Equivalently,
                     a(n+2) = 2*(1+3*n)*a(n+1) - 4*(2*n^2-n-3)*a(n)
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of pre‑computed terms   */
    static size_t cap     = 0;      /* current allocation capacity   */
    static idx_t  high    = 0;      /* largest index already stored  */

    /* initialisation: store first 5 terms (indices 0 … 4) */
    if (cache == NULL) {
        cap = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = (i == 4) ? (seq_t)24 : (seq_t)0;
        high = 4;
    }

    /* if the ask is within known range, return it immediately */
    if (n <= high)
        return cache[n];

    /* enlarge the cache if required */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute missing terms from (high+1) up to n */
    for (idx_t i = high + 1; i <= n; ++i) {
        /* recurrence uses terms a(i-2) and a(i-1) */
        int64_t k        = (int64_t)i - 2;          /* k = i-2 ≥ 3 here */
        int64_t abs_c1   = -k - 3 + 2 * k * k;      /* = 2*k^2 - k - 3 ≥ 0   */
        int64_t abs_c2   = 1 + 3 * k;                /* = 3*k + 1  ≥ 0        */

        seq_t t1 = (seq_t)4 * (seq_t)abs_c1 * cache[k];       /* 4*abs_c1*a(k)   */
        seq_t t2 = (seq_t)2 * (seq_t)abs_c2 * cache[k + 1];   /* 2*abs_c2*a(k+1) */
        cache[i] = t2 - t1;                                   /* a(i) = t2 - t1   */
    }

    high = n;
    return cache[n];
}
