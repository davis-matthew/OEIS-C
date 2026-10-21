/*
 * A078013  Expansion of (1-x)/(1-x+x^3).
 * https://oeis.org/A078013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=a(2)=0; * a(n)=a(n-1)-a(n-3) for n>=3
 * Range:     i8 n <= 38, i16 n <= 78, i32 n <= 158, i64 n <= 315,
 *            i128 n <= 631, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External integer types supplied by the build system. */
/* #include <stdint.h>  // defined by the build. */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1 || n == 2) {
        return (seq_t)0;
    }

    /* Sliding‑window variables:
     *  x = a(i-3),  y = a(i-2),  z = a(i-1). */
    seq_t x = (seq_t)1;   /* a(0)   */
    seq_t y = (seq_t)0;   /* a(1)   */
    seq_t z = (seq_t)0;   /* a(2)   */

    seq_t next;
    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = a(i-1) - a(i-3) */
        next = z - x;

        /* Slide the window forward. */
        x = y;
        y = z;
        z = next;
    }

    return z;   /* z holds a(n) after the loop. */
}
