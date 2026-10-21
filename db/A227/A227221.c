/*
 * A227221  Volume of Johnson square pyramid placed upright on cube (rounded
 *          down) with edge lengths equal to n.
 * https://oeis.org/A227221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n^3 + n^3*sqrt(2)/6 )
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 28, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the volume of a Johnson square pyramid placed upright
   on a cube whose edge length is n, rounding the real volume down to
   an integer. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    const seq_t N = (seq_t)n;

    /* Compute U = n^3, using 128‑bit arithmetic when seq_t is 64‑bit or less */
    seq_t U;
    seq_t Y;
    if (sizeof(seq_t) <= 8) {
        __int128 N128 = (unsigned long long)n;
        __int128 U128 = N128 * N128 * N128;      /* n^3  */
        U = (seq_t)U128;
        __int128 Y128 = U128 * U128 * (__int128)2; /* 2 * U^2 */
        Y = (seq_t)Y128;
    } else {
        /* For larger seq_t types (e.g. 128‑bit or arbitrary precision) */
        U = N * N * N;         /* n^3 as big integer */
        Y = U * U * (seq_t)2;  /* 2 * U^2 as big integer */
    }

    /* floor( sqrt(2) * U / 6 ) = floor( floor_sqrt( 2 * U^2 ) / 6 ) */
    seq_t sqrt_val = floor_sqrt(Y);
    seq_t add      = sqrt_val / (seq_t)6;

    return U + add;
}
