/*
 * A144599  Christoffel word of slope 6/7.
 * https://oeis.org/A144599
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

/* Periodic: a(n) = a(n-13) with the initial 13 terms listed below. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[13] = {
        0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1
    };
    /* Compute n mod 13 safely using a 64‑bit modulus */
    idx_t r = n % (idx_t)13;
    return period[(size_t)r];
}
