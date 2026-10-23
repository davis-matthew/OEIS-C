/*
 * A140184  a(n) = 2*a(n-1) + 16*a(n-2) + 16*a(n-3) for n>3 with a(1)=1,
 *          a(2)=14, a(3)=60.
 * https://oeis.org/A140184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 16*a(n-2) + 16*a(n-3) with initial values a(1)
 *            = 1, a(2) = 14, a(3) = 60. The function returns the n‑th term
 *            for n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types are provided by the build environment */
extern seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n < 1, harmless placeholder */

    /* Return pre‑computed initial values directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)60;

    /* Use variables to hold the last three terms of the recurrence. */
    seq_t t1 = (seq_t)1;   /* a(1)   */
    seq_t t2 = (seq_t)14;  /* a(2)   */
    seq_t t3 = (seq_t)60;  /* a(3)   */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute the next term using the recurrence. */
        next = 2 * t3 + 16 * t2 + 16 * t1;

        /* Shift the terms: t1 becomes a(i-2), t2 becomes a(i-1), t3 becomes a(i). */
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
    return t3;
}
