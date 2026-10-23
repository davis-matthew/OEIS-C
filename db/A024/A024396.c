/*
 * A024396  a(n) = ( Product {k = 1..n} 3*k - 1 ) * ( Sum {k = 1..n}
 *          (-1)^(k+1)/(3*k - 1) ).
 * https://oeis.org/A024396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Product_{k=1..n} (3k-1)) * (Sum_{k=1..n}
 *            (-1)^(k+1)/(3k-1)) Recurrence: a(1)=1, a(2)=3,
 *            a(n)=3*a(n-1)+(3*(n-1)-1)²*a(n-2) for n≥3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types provided by the build system */
extern seq_t a(idx_t n);

/* Compute a(n) directly using the recurrence, without any dynamic memory. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n≥1; below that return 0 (unused). */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t a_prev2 = (seq_t)1; /* holds a(n-2) */
    seq_t a_prev1 = (seq_t)3; /* holds a(n-1) */
    for (idx_t i = 3; i <= n; ++i) {
        /* k = 3*(i-1)-1 = 3i-4 */
        idx_t k_val = 3 * (i - 1) - 1;
        seq_t k = (seq_t)k_val;
        seq_t k_sq = k * k;
        seq_t ai = 3 * a_prev1 + k_sq * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = ai;
    }
    return a_prev1;
}
