/*
 * A166428  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 33*a(n-1) - 560*a(n-11) + 528*a(n-12) + coeff_N(n) *
 *            where coeff_N(0)=coeff_N(1)=1, coeff_N(11)=coeff_N(12)=-1, else
 *            0. * This is derived from the rational generating function: *
 *            (1+x)(1-x^11)/(1-33*x+560*x^11-528*x^12) (OEIS A166428). * The
 *            recurrence is of order 12 with the initial value...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised sequence values   */
    static idx_t  cas = 0;        /* current cached size = number of elements  */

    if (n + 1 > cas) {            /* need more space */
        idx_t new_size = n + 1;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) {             /* allocation failure – return 0 as a safe fallback */
            return (seq_t)0;
        }

        /* initialise first element if not already present */
        if (cas == 0) {
            cache[0] = (seq_t)1;
            cas = 1;
        }

        /* fill missing entries up to index n */
        for (idx_t i = cas; i <= n; ++i) {
            seq_t val = 33 * cache[i - 1];         /* 33*a(i-1)              */
            if (i >= 11) val -= 560 * cache[i - 11]; /* -560*a(i-11)        */
            if (i >= 12) val += 528 * cache[i - 12]; /* +528*a(i-12)        */

            /* apply coefficient from numerator of the generating function */
            if (i == 1)          val += (seq_t)1;
            if (i == 11 || i == 12)  val -= (seq_t)1;

            cache[i] = val;
        }
        cas = new_size;
    }

    return cache[n];
}
