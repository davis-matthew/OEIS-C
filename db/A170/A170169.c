/*
 * A170169  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A170169
 *  Recurrence derived from the generating function:
 *  a(0) = 1
 *  a(n) = p(n) + 14 * Σ_{k=1}^{n} a(n-k)          for n = 1 … 38       (p(0)=1, p(n)=2)
 *  a(n) = 14 * Σ_{k=1}^{38} a(n-k) - 105 * a(n-39)   for n ≥ 39
 *
 *  The recurrence follows from the rational generating function
 *  (t^39 + 2*t^38 + … + 2*t + 1) / (1 - 14*t - 14*t^2 - … - 14*t^38 + 105*t^39). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *seq = NULL;
    static idx_t stored = 0;          /* number of terms already computed */

    /* Expand cache if necessary */
    if (seq == NULL) {
        size_t init = (size_t)n + 1;
        seq = (seq_t*)malloc(init * sizeof(seq_t));
        stored = 0;
    } else if ((size_t)n + 1 > (size_t)stored) {
        size_t newsize = (size_t)n + 1;
        seq = (seq_t*)realloc(seq, newsize * sizeof(seq_t));
        /* No need to initialize new area, we will fill it immediately */
    }

    /* Compute missing terms up to n */
    for (idx_t i = stored; i <= n; ++i) {
        if (i == 0) {
            seq[0] = (seq_t)1;                     /* a(0) = 1 */
        } else if (i <= 38) {
            /* a(n) = p(n) + 14 * Σ_{k=1}^{n} a(n-k)   with p(0)=1, p(n)=2 for 1..38 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += seq[i - k];
            }
            seq[i] = (seq_t)(2) + (seq_t)(14) * sum;
        } else {
            /* a(n) = 14 * Σ_{k=1}^{38} a(n-k) - 105 * a(n-39)  for n ≥ 39 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 38; ++k) {
                sum += seq[i - k];
            }
            seq[i] = (seq_t)(14) * sum - (seq_t)(105) * seq[i - 39];
        }
    }
    stored = n + 1;
    return seq[n];
}
