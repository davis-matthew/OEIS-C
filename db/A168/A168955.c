/*
 * A168955  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 21: a(n) = 36·(a(0)+···+a(n-1)) + 2 a(22) =
 *            36·(a(1)+···+a(21)) – 666·a(0) + 1 For n ≥ 23: a(n) =
 *            36·(a(n-1)+···+a(n-21)) – 666·a(n-22)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Buffers for the last 22 terms (indices modulo 22) */
    seq_t buf[22];
    const size_t M = 22;

    /* Base term a(0) */
    buf[0] = (seq_t)1;
    if (n == 0) return buf[0];

    /* Compute a(1) … a(21) using the simpler recurrence
       a(k) = 36·(sum of previous terms) + 2    */
    seq_t sum_prev = buf[0];          /* sum of a(0) … a(k-1) */
    for (idx_t i = 1; i <= 21 && i <= n; ++i) {
        seq_t curr = ((seq_t)36 * sum_prev) + (seq_t)2;
        buf[i % M] = curr;
        sum_prev += curr;
        if (i == n) return curr;
    }

    /* If the requested index is 22 or less, we are done */
    if (n <= 21) return buf[n % M];

    /* Compute a(22) */
    seq_t sum21 = (seq_t)0;
    for (idx_t k = 1; k <= 21; ++k) {
        sum21 += buf[(22 - k) % M];   /* a(21) … a(1) */
    }
    seq_t newterm = ((seq_t)36 * sum21) - ((seq_t)666 * buf[0]) + (seq_t)1;
    buf[22 % M] = newterm;
    if (n == 22) return newterm;

    /* Compute a(23) … a(n) using the general recurrence */
    for (idx_t i = 23; i <= n; ++i) {
        /* sum of the previous 21 terms a(i-1) … a(i-21) */
        sum21 = (seq_t)0;
        for (idx_t k = 1; k <= 21; ++k) {
            sum21 += buf[(i - k) % M];
        }

        seq_t old_term = buf[i % M];                /* a(i-22) */
        newterm = ((seq_t)36 * sum21) - ((seq_t)666 * old_term);
        buf[i % M] = newterm;
    }

    return buf[n % M];
}
