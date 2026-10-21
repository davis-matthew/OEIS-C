/*
 * A057500  Number of connected labeled graphs with n edges and n nodes.
 * https://oeis.org/A057500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n-1)!/2) * Σ_{j=0}^{n-3} n^j / j! (for n≥1,
 *            a(1)=a(2)=0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute the number of connected labeled graphs with n nodes and n edges */
seq_t a(idx_t n)
{
    /* Base cases: a(1)=0, a(2)=0 */
    if (n <= 2) return 0;

    /* Compute (n-1)! as a seq_t */
    seq_t fact = 1;
    for (idx_t i = 2; i < n; ++i) {
        fact *= (seq_t)i;
    }

    /* Initialize term(0) = (n-1)! and sum = term(0) */
    seq_t term = fact;
    seq_t sum  = term;

    /* Iterate j = 1 .. n-3
       term(j) = term(j-1) * n / j   (exact division) */
    for (idx_t j = 1; j <= n - 3; ++j) {
        term = (term * (seq_t)n) / j;
        sum += term;
    }

    /* Final result: divide the sum by 2 (exact division) */
    return sum / 2;
}
