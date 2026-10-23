/*
 * A170070  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1. For 1≤n≤36 : a(n)=2 + 11⋅Σ_{k=0}^{n-1} a(k). For n=37 :
 *            a(37)=1 + 11⋅Σ_{k=0}^{36} a(k). For n≥38 : a(n)=11⋅Σ_{k=1}^{36}
 *            a(n-k) – 66⋅a(n-37)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by a forward dynamic program with memoisation. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *seq  = NULL;   /* array of a(k) */
    static seq_t *cumul = NULL;  /* cumulative sums cum(k)=Σ_{i=0}^k a(i) */
    static idx_t  len = 0;        /* length of allocated arrays (len = last_index+1) */

    /* Expand cache if necessary */
    if (n + 1 > len) {
        /* New size */
        idx_t newlen = n + 1;
        /* Allocate / reallocate */
        seq    = (seq_t*)realloc(seq,    newlen * sizeof(seq_t));
        cumul = (seq_t*)realloc(cumul, newlen * sizeof(seq_t));
        if (!seq || !cumul) return (seq_t)0;  /* allocation failed – return 0 */

        /* Compute missing entries from the old last index up to n */
        for (idx_t i = len; i <= n; ++i) {
            if (i == 0) {
                seq[0]   = (seq_t)1;
                cumul[0] = seq[0];
            } else if (i <= 36) {
                /* a(i) = 2 + 11 * cum(i-1)   */
                seq[i]   = (seq_t)2 + (seq_t)11 * cumul[i - 1];
                cumul[i] = cumul[i - 1] + seq[i];
            } else if (i == 37) {
                /* a(37) = 1 + 11 * cum(36)   */
                seq[i]   = (seq_t)1 + (seq_t)11 * cumul[i - 1];
                cumul[i] = cumul[i - 1] + seq[i];
            } else {
                /* i >= 38:  a(i) = 11 * (cum(i-1) - cum(i-38)) - 66 * a(i-37) */
                seq_t sum_last36 = cumul[i - 1] - cumul[i - 38];
                seq[i] = (seq_t)11 * sum_last36 - (seq_t)66 * seq[i - 37];
                cumul[i] = cumul[i - 1] + seq[i];
            }
        }
        len = newlen;
    }

    return seq[n];
}
