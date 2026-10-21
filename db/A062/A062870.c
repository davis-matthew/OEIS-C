/*
 * A062870  Number of permutations of degree n with greatest sum of distances.
 * https://oeis.org/A062870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n/2)!)^2 if n is even; a(n) = n * (((n-1)/2)!)^2 if n
 *            is odd
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoised factorials: fac[i] = i!   (i >= 0) */
    static seq_t *fac   = NULL;
    static idx_t  fac_len = 0;   /* number of factorials stored */

    /* k = floor(n/2)  (works for both even and odd n) */
    idx_t k = n / 2;

    /* initialise array with 0! = 1 */
    if (fac_len == 0)
    {
        fac = (seq_t *)malloc(sizeof(seq_t));
        if (!fac) return (seq_t)0; /* allocation failure – not expected */
        fac[0] = (seq_t)1;
        fac_len = 1;
    }

    /* grow array and compute missing factorials up to k */
    if (fac_len <= k)
    {
        seq_t *tmp = (seq_t *)realloc(fac, sizeof(seq_t) * (k + 1));
        if (!tmp) return (seq_t)0;
        fac = tmp;
        for (idx_t i = fac_len; i <= k; ++i)
            fac[i] = fac[i - 1] * (seq_t)i;
        fac_len = k + 1;
    }

    seq_t f   = fac[k];          /* k! */
    seq_t res = f * f;           /* (k!)^2 */

    if (n & 1)                   /* if n is odd */
        res = res * (seq_t)n;    /* multiply by n */

    return res;
}
