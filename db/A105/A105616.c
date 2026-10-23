/*
 * A105616  Column 1 of triangle A105615.
 * https://oeis.org/A105616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A105616
   Implementation using the generating function:
   Sum a(n)*x^n = (E(x)/D(x)-1)/x,
   where D(x)=∑(2n-1)!! x^n and E(x)=∑(2n)!! x^n.
   The coefficients of E/D are computed by formal series division:
   c[0]=1; for k≥1: c[k] = E_k - Σ_{i=1..k} d[i]*c[k-i]   (since d[0]=1).
   Then a(n) = c[n+1].
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *c = NULL;   /* coefficients of E/D          */
    static seq_t *d = NULL;   /* coefficients of D            */
    static seq_t *e = NULL;   /* coefficients of E            */
    static idx_t achieved = 0; /* highest index computed in c   */
    static idx_t capacity = 0; /* allocated array size         */

    /* ensure arrays have at least needed+1 slots */
    idx_t needed = n + 1;  /* we need c[0..n+1]            */
    if (needed + 1 > capacity)
    {
        idx_t newcap = capacity ? capacity * 2 : 128;
        while (newcap <= needed) newcap <<= 1;
        c = (seq_t *)realloc(c, newcap * sizeof(seq_t));
        d = (seq_t *)realloc(d, newcap * sizeof(seq_t));
        e = (seq_t *)realloc(e, newcap * sizeof(seq_t));
        capacity = newcap;
    }

    /* initialise at start */
    if (achieved == 0 && capacity > 0)
    {
        c[0] = (seq_t)1;
        d[0] = (seq_t)1;
        e[0] = (seq_t)1;
        achieved = 0;
    }

    /* compute missing coefficients up to index 'needed' */
    for (idx_t k = achieved + 1; k <= needed; ++k)
    {
        /* d[k] = (2k-1)!!  *  d[k-1]*/
        d[k] = d[k - 1] * (seq_t)(2 * k - 1);
        /* e[k] = (2k)!!   *  e[k-1]*/
        e[k] = e[k - 1] * (seq_t)(2 * k);

        seq_t sum = e[k];
        for (idx_t i = 1; i <= k; ++i)
            sum -= d[i] * c[k - i];
        c[k] = sum;
    }
    achieved = needed;

    return c[n + 1];
}
