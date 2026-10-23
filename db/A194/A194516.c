/*
 * A194516  First coordinate of (3,5)-Lagrange pair for n.
 * https://oeis.org/A194516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n - 5*floor((3*n + 3)/8)
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute floor((3*n + 3)/8) using 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 prod = (unsigned __int128)3 * n + 3;
    unsigned __int128 t    = prod / 8;          /* floor((3*n + 3)/8) */

    /* Compute 2*n and 5*t in 128‑bit to avoid overflow. */
    unsigned __int128 two_n = (unsigned __int128)2 * n;
    unsigned __int128 five_t = (unsigned __int128)5 * t;

    /* Difference may be negative; use signed 128‑bit. */
    signed __int128 diff = (signed __int128)two_n - (signed __int128)five_t;

    return (seq_t)diff;
}
