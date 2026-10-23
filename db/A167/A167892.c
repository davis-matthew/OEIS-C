/*
 * A167892  a(n) = Sum_{k=1..n} Catalan(k)^2.
 * https://oeis.org/A167892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} Catalan(k)^2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t sum = 0;
    seq_t C = 1; /* Catalan(0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* recurrence: C_k = C_{k-1} * 2*(2*(k-1)+1) / (k+1) */
        seq_t t = ((seq_t)2) * ((seq_t)(2 * (k - 1) + 1));
        C = C * t / (seq_t)(k + 1);
        seq_t sq = C * C;
        sum += sq;
    }

    return sum;
}
