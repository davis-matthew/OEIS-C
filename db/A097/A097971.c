/*
 * A097971  Number of alternating runs in all permutations of [n] (the
 *          permutation 732569148 has four alternating runs: 732, 2569, 91 and
 *          148).
 * https://oeis.org/A097971
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (2*n-1) / 3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is not defined; return 0. */
    if (n < 2) {
        return (seq_t)0;
    }

    seq_t fact = (seq_t)1;
    /* Compute factorial n! */
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Multiply by (2*n-1) and divide by 3.  All operations stay in
       the integer domain; the division is exact for every n >= 2. */
    fact *= (seq_t)(2 * n - 1);
    fact /= (seq_t)3;

    return fact;
}
