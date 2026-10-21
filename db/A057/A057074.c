/*
 * A057074  a(n) = floor(12^12/n).
 * https://oeis.org/A057074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(12^12 / n)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   pow12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow12.h"
/* Computes the integer division of 12^12 by the index n. */

seq_t a(idx_t n)
{
    /* a(1) is the first term; if n == 0 return 0 to avoid
       undefined behaviour. */
    if (n == 0)
        return 0;

    /* Lazy‑initialise the constant 12^12 once. */
    static seq_t d = 0;
    if (d == 0)
        d = pow12();

    return d / (seq_t)n;   /* integer division = floor */
}
