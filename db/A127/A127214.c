/*
 * A127214  a(n) = 2^n*tribonacci(n) or (2^n)*A001644(n+1).
 * https://oeis.org/A127214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127214
   a(n) = 2*a(n-1) + 4*a(n-2) + 8*a(n-3)
   with a(1) = 2, a(2) = 12, a(3) = 56
   (OEIS offset 1) */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety */
    if (n == 0)
        return 0;

    /* Handle the first three terms directly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)56;

    /* Sliding window for the recurrence */
    seq_t a1 = (seq_t)2;   /* a(n-3) */
    seq_t a2 = (seq_t)12;  /* a(n-2) */
    seq_t a3 = (seq_t)56;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = (a3 * (seq_t)2) + (a2 * (seq_t)4) + (a1 * (seq_t)8);
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
