/*
 * A194150  a(n) = Sum_{j=1..n} floor(j*(3+sqrt(5))); n-th partial sum of
 *          Beatty sequence for 3+sqrt(5).
 * https://oeis.org/A194150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=1..n} floor(j*(3+sqrt(5)))
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit number.
   Returns floor(sqrt(x)).  Works for x < 2^128. */
static inline __int128 integer_sqrt128(__int128 x)
{
    __int128 r = 0;
    __int128 bit = (__int128)1 << 126;   /* highest power of four <= 2^127 */

    /* bring bit down to the highest power of four <= x */
    while (bit > x) bit >>= 2;

    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;                     /* floor(sqrt(x)) */
}

/* Cached 60‑bit scaled value of sqrt(5): floor(sqrt(5)*2^60). */
static __int128 sqrt5_scaled = 0;
static int sqrt5_ready   = 0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* guard against 0, offset is 1 */

    if (!sqrt5_ready) {
        /* sqrt5_scaled = floor(sqrt(5*2^120)) = floor(sqrt(5)*2^60) */
        sqrt5_scaled = integer_sqrt128(((__int128)5) << 120);
        sqrt5_ready = 1;
    }

    seq_t sum = (seq_t)0;
    for (idx_t j = 1; j <= n; ++j) {
        /* floor(j*sqrt(5)) = ( (j * sqrt5_scaled) >> 60 ) */
        __int128 term_approx = (( __int128)j * sqrt5_scaled) >> 60;
        /* floor(j*(3+sqrt(5))) = 3*j + floor(j*sqrt(5)) */
        __int128 term = (__int128)j * 3 + term_approx;
        sum += (seq_t)term;                 /* seq_t addition (built‑in or user) */
    }
    return sum;
}
