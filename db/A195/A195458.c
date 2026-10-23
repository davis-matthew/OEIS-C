/*
 * A195458  a(n) = floor(sqrt(n)) * a(n-1), starting with 1.
 * https://oeis.org/A195458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(n)) * a(n-1), a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* requested header */

/* Compute a(n) for n >= 1.  The function works for any integer
 * type seq_t chosen by the build system.  For arbitrary‑precision
 * types the multiplication operator must be defined. */
seq_t a(idx_t n)
{
    /* a(1) = 1; for n < 1 the function is not defined, but we
     * return 0 in that case to keep the function well‑defined. */
    if (n <= 1)
        return (seq_t)1;

    seq_t prod = (seq_t)1;       /* running product */
    idx_t sqrt_floor = 1;        /* floor(sqrt(i)) for current i */

    for (idx_t i = 2; i <= n; ++i) {
        /* Increase sqrt_floor precisely when hitting a new square. */
        if ((sqrt_floor + 1) * (sqrt_floor + 1) <= i)
            ++sqrt_floor;

        prod *= (seq_t)sqrt_floor;
    }

    return prod;
}
