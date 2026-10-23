/*
 * A022452  Expansion of 1/((1-x)*(1-5*x)*(1-7*x)*(1-8*x)).
 * https://oeis.org/A022452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 56*a(n-2) + (5^(n+1)-1)/4, initial values:
 *            a(0) = 1, a(1) = 21
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)21;  /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* term = (5^(i+1) - 1) / 4   */
        seq_t term = power5(i + 1);   /* 5^(i+1) */
        term = (term - (seq_t)1) / (seq_t)4;

        /* a(i) = 15*a(i-1) - 56*a(i-2) + term */
        cur = (seq_t)15 * prev1;
        cur -= (seq_t)56 * prev2;
        cur += term;

        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
