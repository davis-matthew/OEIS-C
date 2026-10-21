/*
 * A072130  a(n+1) - 3*a(n) + a(n-1) = (2/3)*(1+w^(n+1)+w^(2*n+2)); a(1) = 0,
 *          a(2) = 1; where w is the cubic root of unity.
 * https://oeis.org/A072130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Lucas(2*n-1) - ((n%3==2)?2:1))/2 where Lucas(0)=2,
 *            Lucas(1)=1 and Lucas(k)=Lucas(k-1)+Lucas(k-2)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *L = NULL;       /* table of Lucas numbers */
    static idx_t Lmax = 0;        /* highest index computed in L */
    static size_t Lcap = 0;       /* allocated size of L */

    if (n == 0) return 0;          /* value is undefined for n<1, return 0 */

    idx_t lucas_idx = 2 * n - 1;    /* index of needed Lucas number */

    /* initialise the table on first call */
    if (L == NULL) {
        Lcap = 4;                  /* start with a small capacity */
        L = (seq_t *)malloc(Lcap * sizeof(seq_t));
        L[0] = (seq_t)2;
        L[1] = (seq_t)1;
        Lmax = 1;
    }

    /* grow capacity if needed */
    if (lucas_idx > Lmax) {
        if ((idx_t)Lcap <= lucas_idx) {
            while ((idx_t)Lcap <= lucas_idx)
                Lcap <<= 1;
            L = (seq_t *)realloc(L, Lcap * sizeof(seq_t));
        }
        /* compute missing Lucas numbers */
        for (idx_t i = Lmax + 1; i <= lucas_idx; ++i)
            L[i] = L[i - 1] + L[i - 2];
        Lmax = lucas_idx;
    }

    /* compute adjustment term */
    seq_t adjust = (n % 3 == 2) ? (seq_t)2 : (seq_t)1;

    /* the result of interest */
    seq_t result = (L[lucas_idx] - adjust) / (seq_t)2;

    return result;
}
