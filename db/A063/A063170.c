/*
 * A063170  Schenker sums with n-th term.
 * https://oeis.org/A063170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..n} n^k * n! / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* compute sum Σ n^k * n! / k! iteratively */
    seq_t term = fact;   /* term for k = 0 : 1 * n! / 0! = n! */
    seq_t sum  = term;
    for (idx_t k = 1; k <= n; ++k) {
        /* term_k = term_{k-1} * n / k  (exact division) */
        term = (term * (seq_t)n) / (seq_t)k;
        sum += term;
    }
    return sum;
}
