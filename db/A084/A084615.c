/*
 * A084615  a(n) = sum of absolute values of coefficients of (1+x-3x^2)^n.
 * https://oeis.org/A084615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of absolute values of coefficients of (1+x-3x^2)^n *
 *            computed as a(n) = sum_{k=0}^{2n} | sum_{c=0}^{floor(k/2)}
 *            binom(n,c) * * (-3)^c * binom(n-c, k-2c) | * * This
 *            implementation keeps separate arrays for the positive and *
 *            negative part while multiplying by (1 + x - 3x^2). * No
 *            floati...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Max degree is 2*n, so we need arrays of length 2*n+1. */
    const size_t len = (size_t)2 * (size_t)n + 1;
    /* Arrays: pos holds the positive part, neg the negative part. */
    seq_t *pos  = alloc_seq_array(len);
    seq_t *neg  = alloc_seq_array(len);
    seq_t *newpos = alloc_seq_array(len);
    seq_t *newneg = alloc_seq_array(len);

    /* Base case: (1 + x - 3x^2)^1 */
    pos[0] = (seq_t)1;
    pos[1] = (seq_t)1;
    neg[2] = (seq_t)3;   /* coefficient -3 */

    /* For i = 2..n perform multiplication by (1 + x - 3x^2). */
    for (idx_t i = 2; i <= n; ++i) {
        size_t prev_len = (size_t)2 * (size_t)(i - 1) + 1;
        size_t new_len  = (size_t)2 * (size_t)i + 1;

        /* Zero-out the new arrays. */
        for (size_t j = 0; j < new_len; ++j) {
            newpos[j] = (seq_t)0;
            newneg[j] = (seq_t)0;
        }

        /* Update rules:
         *  newpos = pos*1 + pos*x + 3*neg*x^2
         *  newneg = neg*1 + neg*x + 3*pos*x^2
         */
        for (size_t k = 0; k < prev_len; ++k) {
            seq_t vpos = pos[k];
            if (vpos) {
                newpos[k]   += vpos;           /* *1  */
                newpos[k+1] += vpos;           /* *x  */
                newneg[k+2] += vpos * (seq_t)3;/* -3x^2  contributes to negative part */
            }

            seq_t vneg = neg[k];
            if (vneg) {
                newpos[k+2] += vneg * (seq_t)3;/* -3x^2 contributes to positive part */
                newneg[k]   += vneg;           /* *1  */
                newneg[k+1] += vneg;           /* *x  */
            }
        }

        /* Swap the arrays: newpos/neg become the current ones. */
        seq_t *tmp = pos; pos = newpos; newpos = tmp;
        tmp = neg; neg = newneg; newneg = tmp;
    }

    /* Compute the sum of absolute values of the final coefficient array. */
    seq_t sum = (seq_t)0;
    for (size_t k = 0; k < len; ++k) {
        seq_t p = pos[k];
        seq_t q = neg[k];
        seq_t abs;
        if (p >= q) {
            abs = p - q;
        } else {
            abs = q - p;
        }
        sum += abs;
    }

    /* Clean up */
    free_seq_array(pos);
    free_seq_array(neg);
    free_seq_array(newpos);
    free_seq_array(newneg);

    return sum;
}
