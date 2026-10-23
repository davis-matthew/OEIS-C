/*
 * A013899  a(n) = 21^(5*n + 2).
 * https://oeis.org/A013899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21^(5*n + 2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes a(n) = 21^(5*n + 2)  (OEIS A013899) */
seq_t a(idx_t n)
{
    /* 21^2 = 441   and   21^5 = 4084101 */
    seq_t result = (seq_t)441;
    seq_t base   = (seq_t)4084101;
    idx_t e = n;

    while (e != 0) {
        if (e & 1) result *= base;   /* multiply result by current base if bit is set */
        base *= base;                /* square base for next bit */
        e >>= 1;                     /* proceed to next bit */
    }

    return result;
}
