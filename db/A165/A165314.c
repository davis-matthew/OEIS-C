/*
 * A165314  a(0)=1, a(1)=6, a(n)=13*a(n-1)-36*a(n-2) for n>1.
 * https://oeis.org/A165314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 13*a(n-1) - 36*a(n-2),  a(0)=1, a(1)=6
 * Recurrence is evaluated iteratively.
 */

#include <stdint.h> /* for uint64_t, etc. */

seq_t a(idx_t n)
{
    /* Handle base cases directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    /* Use previous two values to iterate to n. */
    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)6;  /* a(1) */
    seq_t cur;

    for (idx_t k = 2; k <= n; ++k) {
        cur = (seq_t)13 * prev1 - (seq_t)36 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
