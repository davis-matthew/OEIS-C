/*
 * A126514  Number of base 19 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A126514
   Number of base 19 n‑digit numbers with adjacent digits differing by four or less.
   Recurrence (DP):  a(0)=1, a(1)=19.
   For n≥2, let f_k[d] be the number of length k strings ending with digit d (0≤d≤18).
   f_1[d]=1 for all d.
   f_{k+1}[d] = Σ_{e=max(0,d-4)}^{min(18,d+4)} f_k[e].
   a(k) = Σ_{d=0}^{18} f_k[d].
   The implementation keeps the current f vector and memoises a(k) values.
*/

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state for memoisation        */
    static seq_t *a_vals = NULL;      /* a(0)..a(max_n)          */
    static size_t a_cap = 0;          /* allocated capacity       */
    static idx_t max_n = 0;           /* highest n computed       */
    static seq_t f[19];               /* f vector for current n   */
    static seq_t g[19];               /* temporary for n+1       */

    /* initialise memoisation on first call      */
    if (a_vals == NULL) {
        a_cap = 1;
        a_vals = (seq_t *)malloc(sizeof(seq_t) * a_cap);
        a_vals[0] = (seq_t)1;        /* a(0)=1 */
        max_n = 0;
    }

    if (n <= max_n) {
        return a_vals[n];           /* already computed   */
    }

    /* handle n==1 specially, initialise f to all ones  */
    if (max_n == 0) {
        for (int d = 0; d < 19; ++d) {
            f[d] = (seq_t)1;
        }
        /* a(1) = sum of f  */
        seq_t sum = 0;
        for (int d = 0; d < 19; ++d) {
            sum += f[d];
        }
        /* ensure capacity for a(1) */
        if (a_cap <= 1) {
            a_cap = 2;
            a_vals = (seq_t *)realloc(a_vals, sizeof(seq_t) * a_cap);
        }
        a_vals[1] = sum;
        max_n = 1;
    }

    /* compute successive values until reaching n   */
    while (max_n < n) {
        /* g[d] = new f for next length  */
        for (int d = 0; d < 19; ++d) {
            int start = d - 4;
            if (start < 0) start = 0;
            int end = d + 4;
            if (end > 18) end = 18;
            seq_t s = 0;
            for (int e = start; e <= end; ++e) {
                s += f[e];
            }
            g[d] = s;
        }
        /* copy g into f for next iteration    */
        for (int d = 0; d < 19; ++d) {
            f[d] = g[d];
        }

        /* a(max_n+1) = sum of f */
        seq_t sum = 0;
        for (int d = 0; d < 19; ++d) {
            sum += f[d];
        }

        /* ensure capacity for new a value    */
        if (a_cap <= (size_t)(max_n + 1)) {
            a_cap *= 2;
            if (a_cap == 0) a_cap = 2;   /* safety for tiny initial cap */
            a_vals = (seq_t *)realloc(a_vals, sizeof(seq_t) * a_cap);
        }
        a_vals[max_n + 1] = sum;
        ++max_n;
    }

    return a_vals[n];
}
