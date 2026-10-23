/*
 * A169011  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A169011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for 1 <= n <= 22: a(n) = 2 + 44 * sum_{k=0}^{n-1} a(k)
 *            for n >= 23: a(n) = 44 * sum_{i=1}^{22} a(n-i) - 990 * a(n-23)
 *            (The sequence is defined by the rational generating function
 *            with denominator 1 - 44 t - … - 44 t^22 + 990 t^23.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* Allocate array for the first n+1 terms */
    seq_t *a = alloc_seq((size_t)(n + 1));
    a[0] = (seq_t)1;

    /* Compute terms 1 .. 22 using the prefix‑sum formula */
    if (n > 0) {
        seq_t prefix = a[0];
        for (idx_t m = 1; m <= n && m <= 22; ++m) {
            a[m] = (seq_t)2 + (seq_t)44 * prefix;
            prefix += a[m];
        }
        if (n <= 22) {
            seq_t res = a[n];
            free_seq(a);
            return res;
        }
    }

    /* Prepare sliding window of last 22 terms for the recurrence
       (for n >= 23).  Initially the window is a[1]..a[22]. */
    seq_t window_sum = (seq_t)0;
    for (idx_t i = 1; i <= 22; ++i) {
        window_sum += a[i];
    }

    /* Compute all remaining terms up to n */
    for (idx_t m = 23; m <= n; ++m) {
        seq_t term = (seq_t)44 * window_sum - (seq_t)990 * a[m - 23];
        a[m] = term;

        /* Update the sliding window: add new term, drop term that
           falls out of the 22‑term window. */
        window_sum += a[m];
        window_sum -= a[m - 22];
    }

    seq_t res = a[n];
    free_seq(a);
    return res;
}
