/*
 * A170486  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 43·(a(n-1)+...+a(n-44)) – 946·a(n-45) + b(n) where
 *            b(0)=1, b(n)=2 for 1≤n≤44, b(45)=1, and b(n)=0 for n>45
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Store the last 45 terms in a circular buffer. */
    static seq_t buf[45] = { 0 };
    static idx_t last = (idx_t)-1;          /* no terms computed yet */

    /* Determine the first index to compute. */
    idx_t start = (last == (idx_t)-1) ? 0 : last + 1;

    for (idx_t i = start; i <= n; ++i) {
        seq_t sum = 0;

        /* Sum the 44 previous terms (or fewer if i < 44). */
        for (idx_t k = 1; k <= 44 && k <= i; ++k) {
            sum += buf[(i - k) % 45];
        }

        seq_t term = 43 * sum;          /* 43·sum */

        /* Add b(i). */
        if (i == 0 || i == 45) {
            term += 1;
        } else if (i <= 44) {
            term += 2;
        }

        /* Subtract 946·a(i-45) for i ≥ 45. */
        if (i >= 45) {
            term -= 946 * buf[(i - 45) % 45];
        }

        buf[i % 45] = term;
    }

    last = n;
    return buf[n % 45];
}
