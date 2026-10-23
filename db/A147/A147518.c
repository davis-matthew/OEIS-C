/*
 * A147518  Expansion of (1-x)/(1-4*x-6*x^2).
 * https://oeis.org/A147518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 6*a(n-2) with a(0)=1, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t, seq_t */
#include <stddef.h>   /* for size_t */

/* Compute the OEIS sequence A147518 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Working variables: a(n-2) and a(n-1) */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i) {
        /* current = 4*prev1 + 6*prev2 */
        current = 4 * prev1 + 6 * prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
