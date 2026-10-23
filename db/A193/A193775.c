/*
 * A193775  Number of signed permutations invariant under the reverse
 *          complement and avoiding (-2, -1), (-1, 2), and (1, -2).
 * https://oeis.org/A193775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (⌊n/2⌋)! * 2^{⌊n/2⌋} + Σ_{k=0}^{⌊(n-1)/2⌋} k!
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Use existing utilities. */
#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) = 1, a(1) = 2, a(2) = 3, ... */
    if (n == 0) return (seq_t)1;

    /* m = ⌊n/2⌋ */
    idx_t m = n >> 1;

    /* sum_end = ⌊(n-1)/2⌋ */
    idx_t sum_end = (n - 1) >> 1;

    /* term1 = m! * 2^m */
    seq_t fact_m = factorial(m);
    seq_t term1 = fact_m * pow2(m);

    /* sum of factorials from 0! to sum_end! */
    seq_t sum = 0;
    seq_t fact = 1;            /* fact = 0! initially */
    sum += fact;               /* add 0! = 1 */
    for (idx_t k = 1; k <= sum_end; ++k) {
        fact *= (seq_t)k;      /* fact = k! */
        sum += fact;
    }

    return term1 + sum;
}
