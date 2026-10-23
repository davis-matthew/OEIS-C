/*
 * A183138  a(n) = floor(n/(2+sqrt(2))).
 * https://oeis.org/A183138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/(2+sqrt(2)))
 * Range:     u8 n <= 874, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* √2 ≈ 1.4142135623730950488016887242097
   Here we use a 17‑digit fixed‑point approximation:
   √2 ≈ NUM / DEN,  NUM = 141421356237309504, DEN = 100000000000000000  */
static const __int128 SQRT2_NUM = ( __int128 )141421356237309504ULL;
static const __int128 SQRT2_DEN = ( __int128 )100000000000000000ULL;

seq_t a(idx_t n)
{
    /* Compute floor(n * √2) using the fixed‑point approximation */
    __int128 nn = n;                      /* integer value of n          */
    __int128 b  = (nn * SQRT2_NUM) / SQRT2_DEN;   /* floor(n*√2)                */

    /* a(n) = (2*n - floor(n*√2) - 1) / 2   (all integers) */
    __int128 a_val = ((nn << 1) - b - 1) >> 1;

    return (seq_t)a_val;
}
