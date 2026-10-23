/*
 * A193287  E.g.f.: A(x) = 1/(1 - 2*x^2)^(1 + 1/(2*x)).
 * https://oeis.org/A193287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{m=0..n} ( [x^(n-m)] Π_{k=1..m} (1+2k x) ) / m!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = n! * Sum_{m=0..n} ( (n! / m!) * c(m, n-m) )                 */
/*          where c(m,j) are the elementary symmetric sums of {1,2,…,2m}. */
/*          Computation uses O(n²) integer arithmetic and O(n) memory.      */
#include <stddef.h>              /* for size_t */
#include <stdlib.h>              /* for malloc, free */

#include "../utils/factorial.h"   /* provides seq_t factorial(idx_t) */

/* External linkage */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate array to hold current row of symmetric sums c(m, j) */
    seq_t *cur = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!cur) return (seq_t)0;   /* allocation failure → zero result */

    /* Initialize c(0,0)=1, others zero */
    for (idx_t i = 0; i <= n; ++i) {
        cur[i] = (seq_t)0;
    }
    cur[0] = (seq_t)1;

    seq_t sum = (seq_t)0;

    /* Factorial ratio: n! / m!  (starts with m = 0 → n! / 0! = n!) */
    seq_t fac_ratio = factorial(n);

    for (idx_t m = 0; m <= n; ++m) {
        /* Add contribution of current m */
        seq_t c_val = cur[n - m];        /* c(m, n-m) */
        sum += fac_ratio * c_val;

        /* Prepare for next m unless finished */
        if (m < n) {
            /* Update symmetric sums to next row m+1 */
            for (idx_t j = m + 1; j > 0; --j) {
                /* cur[j] = cur[j] + 2*(m+1)*cur[j-1] */
                cur[j] += ((seq_t)2 * (seq_t)(m + 1)) * cur[j - 1];
            }

            /* Update factorial ratio: (n! / (m+1)!) = (n! / m!) / (m+1) */
            fac_ratio /= (seq_t)(m + 1);
        }
    }

    free(cur);
    return sum;
}
