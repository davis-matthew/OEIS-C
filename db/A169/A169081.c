/*
 * A169081  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n >= 1: a(n) = 18 * Σ_{k=1}^{24} a(n-k) + b(n) -
 *            c(n) where b(n) = 2 for 1 ≤ n ≤ 24, b(0)=1, b(25)=1, b(n)=0
 *            otherwise and c(n) = 171 * a(n-25) for n ≥ 25, c(n)=0 for n <
 *            25. This recurrence follows from the rational generating
 *            function (t^25+2t^24+…+2t+1)/(1-18t-…-18t^24+171t^25)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *buf       = NULL;   /* array of computed terms */
    static idx_t  last      = (idx_t)-1; /* largest index computed */
    static size_t   buf_cap = 0;      /* allocated capacity */
    static seq_t sum24 = 0;           /* sum of last at most 24 terms */

    /* ensure buffer large enough */
    size_t needed_cap = (size_t)n + 1;
    if (buf_cap < needed_cap) {
        seq_t *newbuf = (seq_t *)realloc(buf, needed_cap * sizeof(seq_t));
        if (!newbuf) abort();               /* out of memory */
        buf = newbuf;
        buf_cap = needed_cap;
    }

    /* compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t b;
        if (i == 0)          b = 1;
        else if (i <= 24)    b = 2;
        else if (i == 25)    b = 1;
        else                 b = 0;

        seq_t val = (seq_t)18 * sum24 + b;
        if (i >= 25) val -= (seq_t)171 * buf[i - 25];

        buf[i] = val;

        /* update sliding window of last 24 terms */
        sum24 += val;
        if (i >= 24) sum24 -= buf[i - 24];

        last = i;
    }

    return buf[n];
}
