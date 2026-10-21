/*
 * A078053  Expansion of (1-x)/(1+2*x-2*x^2-2*x^3).
 * https://oeis.org/A078053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + 2*a(n-2) + 2*a(n-3), a(0) = 1, a(1) = -3,
 *            a(2) = 8 (recurrence derived from the generating function
 *            (1-x)/(1+2x-2x^2-2x^3) )
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the first three terms directly. */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)-3;
    if (n == 2U) return (seq_t)8;

    /* Use three sliding variables to compute the recurrence. */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)-3;  /* a(n-2) */
    seq_t a2 = (seq_t)8;   /* a(n-1) */
    seq_t an;              /* a(n)   */

    for (idx_t i = 3U; i <= n; ++i) {
        an = (seq_t)(-2) * a2 + (seq_t)2 * a1 + (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
