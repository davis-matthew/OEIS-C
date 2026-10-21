/*
 * A099528  Row sums of triangle A099527, so that a(n) = Sum_{k=0..n}
 *          coefficient of z^k in (2 + 3*z + z^2)^(n-[k/2]), where [k/2] is
 *          the integer floor of k/2.
 * https://oeis.org/A099528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(2)=17, a(3)=63, a(n)=4*a(n-1) - 2*a(n-2) +
 *            5*a(n-3) - a(n-4) (n>=4)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n-th term of OEIS A099528.  */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static storage for values that have been computed already */
    static seq_t *vals = NULL;      /* array of terms */
    static idx_t  cap   = 0;        /* allocated capacity */
    static idx_t  comp  = 0;        /* number of terms computed (size of vals) */

    /* helper: grow storage if necessary */
    if (vals == NULL) {
        /* initialise to size at least 4 */
        cap = (n < 4 ? 4 : n + 1);
        vals = (seq_t *)malloc(cap * sizeof(seq_t));
        /* base values */
        vals[0] = 1;
        vals[1] = 5;
        vals[2] = 17;
        vals[3] = 63;
        comp  = 4;                  /* indices 0..3 are known */
    } else if (n + 1 > cap) {
        /* double capacity until it fits n+1 */
        while (cap <= n) cap <<= 1;
        vals = (seq_t *)realloc(vals, cap * sizeof(seq_t));
    }

    /* if requested term already computed, return it */
    if (n < comp) return vals[n];

    /* compute successive terms from the last known up to n */
    for (idx_t i = comp; i <= n; ++i) {
        /* compute 4*vals[i-1] + 5*vals[i-3] - (2*vals[i-2] + vals[i-4])  */
        seq_t result = vals[i-1];

        /* 4 * vals[i-1] */
        result = result + result + result + result;

        /* 5 * vals[i-3] */
        seq_t tmp = vals[i-3];
        tmp = tmp + tmp + tmp + tmp + tmp;

        result = result + tmp;

        /* 2 * vals[i-2] + vals[i-4] */
        seq_t sub = vals[i-2];
        sub = sub + sub;
        sub = sub + vals[i-4];

        /* subtract */
        result = result - sub;

        vals[i] = result;
    }
    comp = n + 1;
    return vals[n];
}
