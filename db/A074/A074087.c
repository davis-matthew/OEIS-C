/*
 * A074087  Coefficient of q^1 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(2,3).
 * https://oeis.org/A074087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+2*a(n-2)-12*a(n-3)-9*a(n-4) for n>=5, a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=6, a(4)=33
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow out‑of‑order and repeated calls */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated size of cache          */
    static idx_t  next  = 0;      /* first index not yet computed    */
    static int    init  = 0;      /* initialization flag             */

    /* Initialise cache with known initial terms */
    if (!init) {
        cap = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)33;
        next = 5;
        init = 1;
    }

    /* Return directly if n is one of the base cases */
    if (n < 5)
        return cache[n];

    /* Grow array if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : n + 1;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute subsequent terms up to n */
    for (idx_t i = next; i <= n; ++i) {
        seq_t val =
            ((seq_t)4) * cache[i - 1]
          + ((seq_t)2) * cache[i - 2]
          - ((seq_t)12) * cache[i - 3]
          - ((seq_t)9)  * cache[i - 4];
        cache[i] = val;
    }
    next = n + 1;

    return cache[n];
}
