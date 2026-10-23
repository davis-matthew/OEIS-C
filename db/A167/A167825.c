/*
 * A167825  Subsequence of A167709 whose indices are congruent to 4 mod 5,
 *          i.e., a(n) = A167709(5*n+4).
 * https://oeis.org/A167825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 6, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167825: a(n+2) = 340*a(n+1) - a(n); a(0) = 220; a(1) = 74801 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return (seq_t)220;
    if (n == 1) return (seq_t)74801;

    /* Iteratively compute up to n using the recurrence */
    seq_t a0 = (seq_t)220;          /* a(0) */
    seq_t a1 = (seq_t)74801;        /* a(1) */
    seq_t a2;                       /* to hold a(k) */

    /* Loop from 2 to n */
    for (idx_t i = 2; i <= n; ++i) {
        a2 = 340 * a1 - a0;  /* a(i) = 340*a(i-1) - a(i-2) */
        a0 = a1;
        a1 = a2;
    }

    return a1;  /* a(n) */
}
