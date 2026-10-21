/*
 * A051685  Auxiliary sequence for calculation of number of even permutations
 *          of degree n and order exactly 4.
 * https://oeis.org/A051685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = c(n,4) where c(0,1)=1, c(0,2)=c(0,4)=0 c(n,1)=1 for all
 *            n>=0 c(n,2)=c(n-1,2)-(n-1)*(1+c(n-2,2))
 *            c(n,4)=c(n-1,4)-(n-1)*c(n-2,4)-(n-3)*(n-2)*(n-1)*(1+c(n-4,2)+c(n-4,4))
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 15, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static arrays to memoize c(n,2) and c(n,4) */
    static seq_t *c2 = NULL;
    static seq_t *c4 = NULL;
    static idx_t  cap  = 0;   /* allocated capacity in indices */
    static idx_t  maxn = 0;   /* highest n computed so far */

    if (n == 0)          /* offset starts at 1; return 0 for safety */
        return (seq_t)0;

    /* grow storage if needed */
    if (n + 1 > cap) {
        size_t newcap = (cap == 0 ? 8 : cap * 2);
        while (newcap <= n) newcap <<= 1;
        c2 = (seq_t *)realloc(c2, newcap * sizeof(seq_t));
        c4 = (seq_t *)realloc(c4, newcap * sizeof(seq_t));
        cap = (idx_t)newcap;
    }

    /* initialise base cases at index 0 */
    if (maxn == 0) {
        c2[0] = (seq_t)0;   /* c(0,2)=0   */
        c4[0] = (seq_t)0;   /* c(0,4)=0   */
    }

    /* compute values up to n, starting from maxn+1 */
    for (idx_t i = maxn + 1; i <= n; ++i) {
        /* --- compute c(i,2) --- */
        if (i == 1) {
            c2[1] = (seq_t)0;
        } else {
            /* c(i,2) = c(i-1,2) - (i-1)*(1 + c(i-2,2)) */
            seq_t tmp = c2[i - 1];
            tmp -= (seq_t)(i - 1) * ((seq_t)1 + c2[i - 2]);
            c2[i] = tmp;
        }

        /* --- compute c(i,4) --- */
        /* start with c(i-1,4) - (i-1)*c(i-2,4) */
        seq_t val = c4[i - 1];
        val -= (seq_t)(i - 1) * c4[i - 2];

        if (i >= 4) {
            /* multiply factor (i-3)(i-2)(i-1) */
            seq_t mult = (seq_t)(i - 3);
            mult *= (i - 2);
            mult *= (i - 1);

            /* sum 1 + c(i-4,2) + c(i-4,4)  (c(i-4,1)=1) */
            seq_t sum = (seq_t)1 + c2[i - 4] + c4[i - 4];

            /* subtract mult * sum */
            val -= mult * sum;
        }

        c4[i] = val;
    }

    maxn = n;
    return c4[n];
}
