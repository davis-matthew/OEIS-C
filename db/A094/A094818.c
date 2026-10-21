/*
 * A094818  Number of classes of dp-homogeneous spherical curves with n double
 *          points.
 * https://oeis.org/A094818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A094818 – number of classes of dp‑homogeneous spherical curves with n double points.
   Formula: a(n) = a(n-12) for n > 14, with the initial values below. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* initial values a(0) … a(14) */
    static const seq_t tbl[15] = {
        (seq_t)1, /* a(0) */
        (seq_t)1, /* a(1) */
        (seq_t)2, /* a(2) */
        (seq_t)2, /* a(3) */
        (seq_t)4, /* a(4) */
        (seq_t)2, /* a(5) */
        (seq_t)2, /* a(6) */
        (seq_t)2, /* a(7) */
        (seq_t)4, /* a(8) */
        (seq_t)2, /* a(9) */
        (seq_t)3, /* a(10) */
        (seq_t)2, /* a(11) */
        (seq_t)3, /* a(12) */
        (seq_t)2, /* a(13) */
        (seq_t)3  /* a(14) */
    };

    /* Apply the recurrence a(n)=a(n-12) for all n>14 */
    while (n > 14)
        n -= 12;

    return tbl[(size_t)n];
}
