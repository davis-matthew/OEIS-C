/*
 * A099608  Table of crystal ball sequences for A_n lattices read by
 *          antidiagonals.
 * https://oeis.org/A099608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A099608:  Table of crystal ball sequences for A_n lattices read by antidiagonals.
 *  a(n) is the entry T(i,j) where i+j = d and n = d(d+1)/2 + i with d = ⌊(√(8n+1)-1)/2⌋.
 *  T(i,j) is given by the hypergeometric series
 *      T(i,j) = Σ_{m=0}^{min(i,j)} (i+m)! * j! / ((i-m)! (j-m)! (m!)^3).
 *  The summand satisfies the recurrence
 *      t_0 = 1,
 *      t_m = t_{m-1} * (i+m) * (i-m+1) * (j-m+1) / (m^3)
 *  for 1 ≤ m ≤ min(i,j).  The desired term is the sum of all t_m.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* find diagonal d such that d(d+1)/2 ≤ n < (d+1)(d+2)/2 */
    idx_t d = 0;
    for (;; ++d) {
        idx_t next = (d + 1) * (d + 2) / 2;
        if (next > n) break;
    }
    idx_t base = d * (d + 1) / 2;
    idx_t i = n - base;           /* row index */
    idx_t j = d - i;               /* column index */

    idx_t limit = (i < j) ? i : j;
    seq_t sum = 1;                 /* term t_0 = 1 */
    seq_t term = 1;

    for (idx_t m = 1; m <= limit; ++m) {
        /* multiply by (i+m) * (i-m+1) * (j-m+1) */
        term = term * (seq_t)(i + m);
        term = term * (seq_t)(i - m + 1);
        term = term * (seq_t)(j - m + 1);

        /* divide by m^3 exactly */
        idx_t m3 = m * m * m;
        term = term / (seq_t)m3;

        sum += term;
    }
    return sum;
}
