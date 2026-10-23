/*
 * A169527  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n) = 32 * (a(n-1)+…+a(n-33)) – 528·a(n-34) +
 *            p(n) where p(n)=1 if n=0 or n=34; p(n)=2 if 1≤n≤33; p(n)=0
 *            otherwise. This implements the generating function
 *            (t^34+2t^33+…+2t+1)/(528t^34-32t^33-…-32t+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* array holding computed terms up to n */
    seq_t *arr = seq_alloc_array(n + 1);
    arr[0] = (seq_t)1;

    /* compute terms sequentially */
    for (idx_t i = 1; i <= n; ++i) {
        /* inhomogeneous part from the numerator */
        seq_t p = (seq_t)0;
        if (i <= 34) {
            p = (i == 34) ? (seq_t)1 : (seq_t)2;
        }

        /* sum the last 33 terms */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= 33 && i >= k; ++k) {
            sum += arr[i - k];
        }

        /* recurrence contribution */
        seq_t term = (seq_t)32 * sum;
        if (i >= 34) {
            term -= (seq_t)528 * arr[i - 34];
        }

        arr[i] = term + p;
    }

    return arr[n];
}
