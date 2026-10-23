/*
 * A040313  Continued fraction for sqrt(332).
 * https://oeis.org/A040313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued‑fraction terms of √332.
   a(0) = 18, a(n) = {4,1,1,8,1,1,4,36}[ (n-1) mod 8 ] for n > 0. */
#include <stddef.h>   /* for size_t */

static const seq_t period[8] = {4, 1, 1, 8, 1, 1, 4, 36};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)18;

    /* n ≥ 1 here; (n-1) is non‑negative. */
    idx_t pos = (n - 1) % 8;
    return period[(size_t)pos];
}
