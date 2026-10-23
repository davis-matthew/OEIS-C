/*
 * A170277  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*a(n-1) + 26*a(n-2) + … + 26*a(n-40) – 351*a(n-41) +
 *            c(n) where c(0)=1, c(41)=1, c(n)=2 for 1≤n≤40, and c(n)=0
 *            otherwise. This converts the rational generating function (see
 *            OEIS entry). The function computes the sequence in O(n) time and
 *            O(n) space, memoizing all computed terms for arbi...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The following types are supplied by the build system:
   typedef <unsigned integer type> seq_t;   // e.g. uint32_t, uint64_t, unsigned __int128
   typedef <unsigned integer type> idx_t;   // e.g. uint64_t
   They are not defined here.               */

seq_t a(idx_t n)
{
    /* static memoization buffer */
    static seq_t *buf = NULL;      /* array of a(0) … a(current_max) */
    static idx_t  bufsize = 0;     /* number of elements allocated */

    if (n >= bufsize) {             /* need to grow buffer */
        idx_t oldsize = bufsize;
        bufsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, (size_t)bufsize * sizeof(seq_t));
        if (!tmp) abort();          /* unlikely but safe */
        buf = tmp;

        /* initialise new entries from oldsize up to n */
        for (idx_t i = oldsize; i <= n; ++i) {
            seq_t sum = 0;
            /* add 26 * a(i-k) for k = 1 … 40 (only if k ≤ i) */
            for (idx_t k = 1; k <= 40 && k <= i; ++k) {
                sum += (seq_t)26 * buf[i - k];
            }
            /* subtract 351 * a(i-41) if i ≥ 41 */
            if (i >= 41) {
                sum -= (seq_t)351 * buf[i - 41];
            }
            /* add the inhomogeneous term c(i) */
            seq_t c = 0;
            if (i == 0 || i == 41) {
                c = (seq_t)1;
            } else if (i <= 40) {
                c = (seq_t)2;
            }
            buf[i] = sum + c;
        }
    }
    return buf[n];
}
