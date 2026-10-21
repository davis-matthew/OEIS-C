/*
 * A233449  a(n) = Sum_{k=0..n} k! * 2^(n-k).
 * https://oeis.org/A233449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + n! (a(0)=1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoization storage */
    static seq_t *seq  = NULL;
    static size_t  cap = 0;
    static size_t  len = 0;

    static seq_t fac = 1;          /* factorial of the last computed index */

    /* initialise for the first call */
    if (len == 0) {
        cap = 1;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return 0;           /* allocation failed */
        seq[0] = (seq_t)1;            /* a(0) = 1 */
        len = 1;
        fac = (seq_t)1;               /* 0! = 1 */
    }

    /* if already computed, return the cached value */
    if (n < len)
        return seq[n];

    /* grow the array while maintaining enough capacity */
    if (cap <= n) {
        size_t newcap = cap ? cap * 2 : 1;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        seq = tmp;
        cap = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        fac = fac * (seq_t)i;                  /* fac = i! */
        seq[i] = seq[i - 1] * ((seq_t)2) + fac;/* a(i) = 2*a(i-1) + i! */
    }
    len = n + 1;

    return seq[n];
}
