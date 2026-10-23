/*
 * A036833  Schoenheim bound L_1(n,6,5).
 * https://oeis.org/A036833
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A036833 – Schoenheim bound L_1(n,6,5).  
 * The bound is defined recursively by:
 *
 *   L_1(v,k,1) = ceil(v / k)
 *   L_1(v,k,t) = ceil( (v / k) * L_1(v-1, k-1, t-1) )   for t > 1
 *
 *   a(n) = L_1(n,6,5)   for n ≥ 6
 *
 * The implementation evaluates this recurrence iteratively,
 * keeping the values for the current and previous v in two
 * small static tables.  All arithmetic is performed with the
 * prescribed integer type seq_t; divisions are performed
 * with the ceiling rule (a + b – 1) / b.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* tables for k = 1..6, t = 1..k */
    static seq_t prev[7][7];
    static seq_t cur[7][7];
    /* initialise to zero */
    for (int k = 0; k <= 6; ++k)
        for (int t = 0; t <= 6; ++t)
            prev[k][t] = (seq_t)0;

    /* iterate over v = 1 .. n */
    for (idx_t v = 1; v <= n; ++v) {
        for (int k = 1; k <= 6; ++k) {
            for (int t = 1; t <= k; ++t) {
                if (t == 1) {
                    seq_t num = (seq_t)v;
                    seq_t den = (seq_t)k;
                    cur[k][t] = (num + den - (seq_t)1) / den;
                } else if (v >= (idx_t)t) {
                    seq_t num = (seq_t)v * prev[k - 1][t - 1];
                    seq_t den = (seq_t)k;
                    cur[k][t] = (num + den - (seq_t)1) / den;
                } else {
                    cur[k][t] = (seq_t)0;
                }
            }
        }
        /* copy current row to previous for next iteration */
        for (int k = 1; k <= 6; ++k)
            for (int t = 1; t <= k; ++t)
                prev[k][t] = cur[k][t];
    }

    /* a(n) = L_1(n,6,5) */
    return prev[6][5];
}
