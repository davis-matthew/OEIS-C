/*
 * A176965  a(n) = 2^(n-1) - (2^n*(-1)^n + 2)/3.
 * https://oeis.org/A176965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A176965
   Recurrence: a(1)=1, a(2)=0, a(3)=6
   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3)   for n ≥ 4
   (OEIS formula: a(n) = 2^(n-1) – (2^n*(-1)^n + 2)/3) */
#include <stddef.h>   /* idx_t, seq_t are supplied by the build */

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 as a sentinel. */
    if (n == 0)
        return (seq_t)0;

    /* Handle the first three values directly. */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)0;
    if (n == 3)
        return (seq_t)6;

    /* Use the recurrence iteratively, keeping only the last three terms. */
    seq_t a0 = (seq_t)1;   /* a(1) */
    seq_t a1 = (seq_t)0;   /* a(2) */
    seq_t a2 = (seq_t)6;   /* a(3) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t a3 = a2 + (seq_t)4 * a1 - (seq_t)4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
