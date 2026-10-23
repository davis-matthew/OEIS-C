/*
 * A165702  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1 * a(1)=46 * a(2)=2070 * a(3)=93150 * a(4)=4191750 *
 *            a(5)=188628750 * a(6)=8488293750 * a(7)=381973218750 *
 *            a(8)=17188794843750 * a(9)=773495767967715 *
 *            a(n)=44*(a(n-1)+...+a(n-8)) - 990*a(n-9) for n>=10
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[10] = {
    (seq_t)1ULL,                  /* a(0) */
    (seq_t)46ULL,                 /* a(1) */
    (seq_t)2070ULL,               /* a(2) */
    (seq_t)93150ULL,              /* a(3) */
    (seq_t)4191750ULL,            /* a(4) */
    (seq_t)188628750ULL,          /* a(5) */
    (seq_t)8488293750ULL,         /* a(6) */
    (seq_t)381973218750ULL,      /* a(7) */
    (seq_t)17188794843750ULL,    /* a(8) */
    (seq_t)773495767967715ULL    /* a(9) */
};

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *buf   = NULL;   /* array of previously computed terms   */
    static size_t  cap   = 0;     /* allocated capacity of buf            */
    static idx_t   max_n = (idx_t)-1; /* highest index calculated so far     */

    /* initialise cache if this is the first call */
    if (buf == NULL) {
        cap = 16;                               /* minimal size (>=10)               */
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (buf == NULL) return (seq_t)0;        /* out of memory – return 0          */
        for (size_t i = 0; i < 10; ++i)
            buf[i] = init_vals[i];
        for (size_t i = 10; i < cap; ++i)
            buf[i] = (seq_t)0;                  /* initialise rest (not strictly needed) */
        max_n = 9;                                 /* indices 0..9 are valid            */
    }

    /* grow buffer if requested index is beyond current capacity */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;                 /* double until big enough            */
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* out of memory – return 0          */
        /* initialise new elements to zero */
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        buf = tmp;
        cap = new_cap;
    }

    /* if n already computed, just return it */
    if (n <= (idx_t)max_n)
        return buf[(size_t)n];

    /* compute missing values up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i >= 10) {
            /* a(i) = 44*(a(i-1)+...+a(i-8)) - 990*a(i-9) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 8; ++k)
                sum += buf[(size_t)(i - k)];
            buf[(size_t)i] = (seq_t)44 * sum - (seq_t)990 * buf[(size_t)(i - 9)];
        } else {
            /* i <= 9 – we already have the initial values */
            buf[(size_t)i] = init_vals[(size_t)i];
        }
    }
    max_n = n;
    return buf[(size_t)n];
}
