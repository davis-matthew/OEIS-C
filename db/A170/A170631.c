/*
 * A170631  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n) = c(n) + 44*Σ_{i=1}^{min(47,n)} a(n-i) -
 *            (n≥48 ? 990*a(n-48) : 0) where c(0)=c(48)=1, c(1)…c(47)=2,
 *            otherwise c(n)=0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *buf      = NULL;   /* cache of values */
    static size_t   cap    = 0;      /* allocated capacity */
    static idx_t    last   = 0;      /* highest index already computed */
    static bool     ready  = false;  /* cache initialised? */
    static seq_t   sum47   = 0;      /* sum of last 47 computed terms */

    /* ensure enough capacity */
    if (n + 1 > cap)
    {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp;
        if (buf == NULL)
            tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        else
            tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        buf = tmp;
        cap = newcap;
    }

    /* initialise cache if first request */
    if (!ready)
    {
        buf[0] = 1;          /* a(0) */
        sum47  = 0;          /* no previous terms */
        last   = 0;
        ready  = true;
    }

    /* compute any missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        /* update sliding‑window sum of previous 47 terms */
        sum47 += buf[i - 1];
        if (i >= 48)
            sum47 -= buf[i - 48];

        /* determine the constant term c(i) */
        seq_t c;
        if (i == 0 || i == 48)
            c = 1;
        else if (i <= 47)
            c = 2;
        else
            c = 0;

        /* temp = c(i) + 44 * sum47 */
        seq_t temp = c + (seq_t)44 * sum47;

        if (i >= 48)
            buf[i] = temp - (seq_t)990 * buf[i - 48];
        else
            buf[i] = temp;
    }

    last = n;
    return buf[n];
}
