/*
 * A041186  Numerators of continued fraction convergents to sqrt(104).
 * https://oeis.org/A041186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041186
   Formula: a(n) = 102*a(n-2) - a(n-4)
   Initial values: a(0)=10, a(1)=51, a(2)=1030, a(3)=5201
   Each call recomputes the required term using only integer arithmetic
   in type seq_t, with no dynamic allocation. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)10;
    if (n == 1) return (seq_t)51;
    if (n == 2) return (seq_t)1030;
    if (n == 3) return (seq_t)5201;

    /* Iteratively compute using the recurrence:
       a(k) = 102*a(k-2) - a(k-4).  Keep the last four values. */
    seq_t a0 = (seq_t)10;    /* a(k-4) */
    seq_t a1 = (seq_t)51;    /* a(k-3) */
    seq_t a2 = (seq_t)1030;  /* a(k-2) */
    seq_t a3 = (seq_t)5201;  /* a(k-1) */
    seq_t new_val;

    for (idx_t i = 4; i <= n; ++i) {
        new_val = (seq_t)102 * a2;   /* 102*a(k-2) */
        new_val = new_val - a0;      /* - a(k-4) */

        /* Shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }
    return a3;   /* a(n) is the last computed value */
}
