/*
 * A052645  E.g.f. 2*x^2*(1+x-x^2)/(1-x).
 * https://oeis.org/A052645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0, n < 2; a(2) = 4; a(3) = 24; a(n) = 2 * n! for n ≥ 4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) using a cached factorial table. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for caching factorials */
    static seq_t *fac = NULL;     /* fac[i] = i! */
    static idx_t fac_max = 0;     /* highest index computed */
    static idx_t fac_init = 0;    /* flag for first call */

    /* initialize factorial table with 0! = 1 and 1! = 1 */
    if (!fac_init) {
        fac = (seq_t *)malloc(2 * sizeof(seq_t));   /* indices 0 and 1 */
        fac[0] = (seq_t)1;
        fac[1] = (seq_t)1;
        fac_max = 1;   /* we have computed up to 1! */
        fac_init = 1;
    }

    /* grow the table to cover up to n, computing factorials */
    if (n > fac_max) {
        fac = (seq_t *)realloc(fac, (n + 1) * sizeof(seq_t));
        for (idx_t i = fac_max + 1; i <= n; ++i) {
            fac[i] = fac[i - 1] * (seq_t)i;   /* i! = (i-1)! * i */
        }
        fac_max = n;
    }

    /* sequence values */
    if (n < 2) {
        return (seq_t)0;
    } else if (n == 2) {
        return (seq_t)4;
    } else if (n == 3) {
        return (seq_t)24;
    } else {                    /* n ≥ 4 */
        return fac[n] * (seq_t)2;
    }
}
