/*
 * A015596  a(n) = 11 a(n-1) + 4 a(n-2).
 * https://oeis.org/A015596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(1)=1; a(n)=11*a(n-1)+4*a(n-2)  (n≥2) */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Iteratively compute a(n) without any dynamic allocation. */
    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t curr  = 0;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)11 * prev1 + (seq_t)4 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
