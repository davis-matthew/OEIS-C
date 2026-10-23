/*
 * A168941  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A168941
 * Recurrence:
 *   a(0) = 1
 *   for 1 ≤ n ≤ 21:  a(n) = 2 + 22 * Σ_{i=0}^{n-1} a(i)
 *   for n ≥ 22:      a(n) = 22 * Σ_{i=n-21}^{n-1} a(i) - 253 * a(n-22)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;       /* array of computed values */
    static size_t cap   = 0;        /* allocated capacity */
    static idx_t  len   = 0;        /* number of values stored (len = highest index + 1) */

    /* Return already computed value if available */
    if (n < len)
        return seq[n];

    /* Grow the array to accommodate index n */
    if (n >= cap) {
        size_t new_cap = cap ? cap << 1 : 64;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute values from current length up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0) {               /* a(0) = 1 */
            seq[0] = 1;
            continue;
        }

        /* Sum of the previous terms required for the recurrence */
        seq_t sum = 0;
        idx_t lim = (i < 21) ? i : 21;   /* if i<21, sum i terms, else 21 terms */
        for (idx_t j = 1; j <= lim; ++j)
            sum += seq[i - j];

        if (i < 22) {              /* 1 ≤ i ≤ 21 */
            seq[i] = 2 + (seq_t)22 * sum;
        } else {                   /* i ≥ 22 */
            seq_t term = (seq_t)22 * sum;
            seq_t subtract = (seq_t)253 * seq[i - 22];
            seq[i] = term - subtract;
        }
    }

    len = n + 1;   /* update stored length */
    return seq[n];
}
