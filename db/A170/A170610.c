/*
 * A170610  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence:
   a(0) = 1.
   For n ≥ 0:
     a(n) = 23 * sum_{i=max(0, n-47)}^{n-1} a(i)
            - 276 * a(n-48)            // a(k)=0 when k<0
            + b(n),
   where b(0) = b(48) = 1, b(n) = 2 for 1 ≤ n ≤ 47, and b(n) = 0 otherwise. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* cache of computed terms */
    static idx_t size = 0;      /* number of terms cached (next free index) */
    static seq_t sum_window = 0;/* sum of last up to 47 stored terms */

    /* Extend cache if necessary */
    if (size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq = (seq_t*)realloc(seq, new_size * sizeof(seq_t));
        if (seq == NULL) exit(1); /* allocation failed */

        /* Compute new terms from old size to n */
        for (idx_t k = size; k <= n; ++k) {
            /* b(k) term */
            seq_t bk;
            if (k == 0 || k == 48) {
                bk = 1;
            } else if (k <= 47) {
                bk = 2;
            } else {
                bk = 0;
            }

            /* Core recurrence */
            seq_t term = 23 * sum_window;
            if (k >= 48) term -= 276 * seq[k - 48];
            term += bk;
            seq[k] = term;

            /* Update sliding window for next k+1 */
            sum_window += seq[k];
            if (k >= 47) sum_window -= seq[k - 47];
        }
        size = n + 1;
    }
    return seq[n];
}
