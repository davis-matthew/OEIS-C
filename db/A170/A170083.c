/*
 * A170083  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (order 37):
   a(n) = 24·(a(n‑1)+…+a(n‑36)) – 300·a(n‑37) + t(n),
   where t(n)=1 for n=0 or 37,
            t(n)=2 for 1≤n≤36,
            t(n)=0 otherwise.
   Offset 0.  */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* dynamic cache of already computed terms */
    static seq_t *buf = NULL;    /* array of terms a(0)…a(last)      */
    static size_t cap = 0;       /* current capacity of buf          */
    static idx_t last = (idx_t)-1;  /* highest index computed (none) */

    /* enlarge buffer if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = (size_t)(n + 1);
        seq_t *nbuf = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!nbuf) abort();                 /* insure no NULL pointer */
        if (buf) {
            for (size_t i = 0; i < cap; ++i)
                nbuf[i] = buf[i];
            free(buf);
        }
        buf = nbuf;
        cap = newcap;
    }

    /* initialise first term */
    if (last == (idx_t)-1) {
        buf[0] = (seq_t)1;
        last   = 0;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;

        /* constant part t(i) */
        if (i == 0 || i == 37)
            sum += (seq_t)1;
        else if (i >= 1 && i <= 36)
            sum += (seq_t)2;

        /* contributions from the previous 36 terms */
        for (idx_t k = 1; k <= 36; ++k) {
            if (i >= k)
                sum += (seq_t)24 * buf[i - k];
        }

        /* contribution of a(i‑37) */
        if (i >= 37)
            sum += (seq_t)-300 * buf[i - 37];

        buf[i] = sum;
    }

    last = n;
    return buf[n];
}
