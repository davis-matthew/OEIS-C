/*
 * A157084  Consider all consecutive integer Pythagorean quintuples (X, X+1,
 *          X+2, Z-1, Z) ordered by increasing Z; sequence gives X values.
 * https://oeis.org/A157084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=10; a(n)=10*a(n-1)-a(n-2)+8 for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly. */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)10;

    /* Iteratively build up the sequence using the recurrence. */
    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)10;  /* a(n-1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i) {
        current = (seq_t)10 * prev1 - prev2 + (seq_t)8;
        prev2 = prev1;
        prev1 = current;
    }

    return current;  /* a(n) */
}
