/*
 * A019450  Conjectured formula for irreducible 5-fold Euler sums of weight
 *          2n+13.
 * https://oeis.org/A019450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A019450.
   The generating function is
     x*(1 + 2x + 3x^2 + 3x^3 + 2x^4) / ((1 - x^2)^2 (1 - x^3)^2 (1 - x^5)).
   From this we obtain the linear recurrence
     a(n) = rhs(n) + Σ_{k: d_k<0} (-d_k) * a(n-k)
               - Σ_{k: d_k>0} d_k * a(n-k),
   where
     rhs(1..5) = {1, 2, 3, 3, 2}  (rhs(n)=0 for n>5)
     d = {1,0,-2,-2,1,3,1,0,0,-1,-3,-1,2,2,0,-1};
   Note d[0] = 1 (the coefficient of a(n)).
*/

#include <stdlib.h>
#include <stdint.h>

/* Coefficient array d[0..15] for the recurrence. */
static const int d_coeff[16] = {
    1,  0, -2, -2,  1,  3,  1,  0,  0, -1, -3, -1,  2,  2,  0, -1
};

/* rhs[n] for n up to 5; rhs[0] unused. */
static const int rhs_coeff[6] = {
    0, 1, 2, 3, 3, 2
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined for n>=1, but return 0 for safety. */

    static seq_t *cache   = NULL;   /* cache[0] = 0, cache[1..] = a(1..) */
    static idx_t  last    = 0;      /* largest computed index in cache */
    static size_t cap     = 0;      /* allocated length of cache */

    /* Allocate initial cache. */
    if (cache == NULL) {
        cap  = 2;                  /* minimal capacity, will expand later */
        cache = (seq_t *)malloc((cap+1) * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failure -> 0 */
        cache[0] = 0;
    }

    /* Resize if needed. */
    if ((idx_t)n > (idx_t)cap) {
        size_t newcap = cap;
        if (newcap == 0) newcap = 1;
        while (newcap < (size_t)n) {
            newcap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, (newcap+1) * sizeof(seq_t));
        if (tmp == NULL)
            return 0;          /* allocation failure */
        cache = tmp;
        /* Initialize new slots to 0. */
        for (size_t i = cap + 1; i <= newcap; ++i)
            cache[i] = 0;
        cap = newcap;
    }

    /* Compute new terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t rhs = (i <= 5) ? (seq_t)rhs_coeff[(int)i] : (seq_t)0;
        seq_t pos_sum = 0;   /* sum of d_k > 0 terms */
        seq_t neg_sum = 0;   /* sum of (-d_k) for d_k < 0 terms */

        for (int k = 1; k <= 15; ++k) {
            if (i < (idx_t)k)          /* a(i-k) = 0 for i<k */
                continue;
            seq_t term = cache[i - k];
            int coeff = d_coeff[k];
            if (coeff > 0) {
                pos_sum += (seq_t)coeff * term;
            } else if (coeff < 0) {
                neg_sum += (seq_t)(-coeff) * term;
            }
        }

        seq_t val = rhs + neg_sum;
        val -= pos_sum;
        cache[i] = val;
    }

    last = n;
    return cache[n];
}
